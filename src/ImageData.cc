#include "ImageData.h"

ImageData::ImageData(const Napi::CallbackInfo& info) : Napi::ObjectWrap<ImageData>(info) {
  Napi::Number width = info[0].As<Napi::Number>();
  Napi::Number height = info[1].As<Napi::Number>();
  Napi::Buffer<uint8_t> data = info[2].As<Napi::Buffer<uint8_t>>();

  width_ = width.Int32Value();
  height_ = height.Int32Value();
  data_ = data.Data();
}

Napi::FunctionReference ImageData::constructor;

// ==================== Methods for ImageData ====================

size_t ImageData::getWidth() {
  return width_;
}

size_t ImageData::getHeight() {
  return height_;
}

void* ImageData::getData() {
  return data_;
}

// ==================== Methods for NAPI ====================

Napi::Object ImageData::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "ImageData", {
      InstanceAccessor<&ImageData::GetWidth>("width"),
      InstanceAccessor<&ImageData::GetHeight>("height"),
      InstanceAccessor<&ImageData::GetData>("data"),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("ImageData", func);

  return exports;
}

Napi::Value ImageData::CreateInstance(Napi::Env env, size_t width, size_t height, void* data) {
    return constructor.New({
      Napi::Number::New(env, width),
      Napi::Number::New(env, height),
      Napi::Buffer<uint8_t>::Copy(env, reinterpret_cast<uint8_t*>(data), width * height * 4)
    });
}

Napi::Value ImageData::GetWidth(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->width_);
}

Napi::Value ImageData::GetHeight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->height_);
}

Napi::Value ImageData::GetData(const Napi::CallbackInfo& info) {
  size_t len = this->width_ * this->height_ * 4;
  auto buffer = Napi::Uint8Array::New(info.Env(), len, napi_uint8_clamped_array);
  
  for (size_t i = 0; i < len; i++) {
    buffer[i] = (reinterpret_cast<uint8_t*>(this->data_))[i];
  }

  return buffer;

  // Napi::ArrayBuffer ab = Napi::ArrayBuffer::New(info.Env(), this->data_, this->width_ * this->height_ * 4);
  // return Napi::Uint8Array::New(info.Env(), this->width_ * this->height_, ab, 0, napi_uint8_clamped_array);
  // return Napi::Buffer<unsigned char>::Copy(info.Env(), (unsigned char *)this->data_, this->width_ * this->height_ * 4);
}
