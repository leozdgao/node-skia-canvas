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
  // napi_value result;
  // napi_status status;
  // void* data;
  // status = napi_create_buffer_copy(info.Env(), this->len_, &this->data_, &data, &result);

  // return Napi::Value::From(info.Env(), result);
  // return Napi::Buffer<void>::New(info.Env(), this->data_, this->len_);
}

void Image::SetSource(const Napi::CallbackInfo& info, const Napi::Value& value) {
  napi_status status;
  void* data;
  size_t len;

  status = napi_get_buffer_info(info.Env(), value, &data, &len);
  data_ = data;
  len_ = len;

  sk_sp<SkImage> img = SkImage::MakeFromEncoded(
      SkData::MakeWithoutCopy(data, len)
  );

  width_ = img->width();
  height_ = img->height();
}
