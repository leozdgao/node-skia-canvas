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
    if (data_ == nullptr) {
        return info.Env().Undefined();
    }

    return Napi::Buffer<unsigned char>::Copy(info.Env(), (unsigned char *)data_, len_);
}

void Image::SetSource(const Napi::CallbackInfo& info, const Napi::Value& value) {
  if (value.IsNull()) {
    data_ = nullptr;
    image_ = nullptr;
    len_ = 0;
    width_ = 0;
    height_ = 0;
  } else {
    Napi::Buffer buffer = value.As<Napi::Buffer<unsigned char>>();

    if (data_ != nullptr) {
        free(data_);
    }
    
    data_ = malloc(buffer.Length() * sizeof(unsigned char));
    memcpy(data_, buffer.Data(), buffer.Length() * sizeof(unsigned char));
    len_ = buffer.Length();

    image_ = SkImage::MakeFromEncoded(
        SkData::MakeWithoutCopy(data_, len_)
    );

    width_ = image_->width();
    height_ = image_->height();
  }
}

sk_sp<SkImage> Image::getImage() {
  return image_;
}
