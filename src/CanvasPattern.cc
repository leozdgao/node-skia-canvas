#include "Canvas.h"
#include "CanvasPattern.h"
#include "Image.h"
#include "StyleParser.h"

CanvasPattern::CanvasPattern(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CanvasPattern>(info) {
  Napi::Value arg0 = Napi::Value::From(info.Env(), info[0]);
  Napi::Value arg1 = Napi::Value::From(info.Env(), info[1]);
  string repetition = arg1.ToString().Utf8Value();
  
  node_skia::StyleParser::fillTileModeWithRepetition(repetition, repetition_);

  if (arg0.IsObject()) {
    Napi::Object dye = arg0.As<Napi::Object>();

    if (dye.InstanceOf(Image::constructor.Value())) {
      img_ = Napi::ObjectWrap<Image>::Unwrap(dye);
    } else if (dye.InstanceOf(Canvas::constructor.Value())) {
      canvas_ = Napi::ObjectWrap<Canvas>::Unwrap(dye);
    }
  }
}

Napi::FunctionReference CanvasPattern::constructor;

sk_sp<SkShader> CanvasPattern::toShader(bool smoothing) {
  sk_sp<SkImage> image = nullptr;

  if (img_ != nullptr) {
    image = img_->getImage();
  } else if (canvas_ != nullptr) {
    image = canvas_->getSurface()->makeImageSnapshot(
      SkIRect::MakeWH(canvas_->getWidth(), canvas_->getHeight())
    );
  }

  if (image != nullptr) {
    SkSamplingOptions opts = smoothing ? SkSamplingOptions(SkFilterQuality::kHigh_SkFilterQuality) : SkSamplingOptions();
    return image->makeShader(repetition_[0], repetition_[1], opts);
  }
  
  return nullptr;
}


// =================== Wrap Method ===================

Napi::Object CanvasPattern::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "CanvasPattern", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CanvasPattern", func);

  return exports;
}
