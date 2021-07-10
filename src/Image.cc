#include "include/core/SkImage.h"
#include "Image.h"

Image::Image(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Image>(info) {

}

Napi::FunctionReference Image::constructor;

Napi::Object Image::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Image", {
    InstanceAccessor<&Image::GetWidth>("width"),
    InstanceAccessor<&Image::GetHeight>("height"),
    InstanceAccessor<&Image::GetSource, &Image::SetSource>("src"),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Image", func);

  return exports;
}

Napi::Value Image::GetWidth(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->width_);
}

Napi::Value Image::GetHeight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->height_);
}

Napi::Value Image::GetSource(const Napi::CallbackInfo& info) {
  return Napi::Buffer<unsigned char>::Copy(info.Env(), (unsigned char *)data_, len_);
}

void Image::SetSource(const Napi::CallbackInfo& info, const Napi::Value& value) {
  napi_status status;

  Napi::Buffer buffer = value.As<Napi::Buffer<unsigned char>>();
  data_ = buffer.Data();
  len_ = buffer.Length();

  sk_sp<SkImage> img = SkImage::MakeFromEncoded(
      SkData::MakeWithoutCopy(data_, len_)
  );

  width_ = img->width();
  height_ = img->height();
}
