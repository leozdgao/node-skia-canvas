#pragma once

#include <napi.h>

class ImageData : public Napi::ObjectWrap<ImageData> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Value CreateInstance(Napi::Env env, size_t width, size_t height, void* data);

  ImageData(const Napi::CallbackInfo& info);

  size_t getWidth();
  size_t getHeight();
  void* getData();

private:
  static Napi::FunctionReference constructor;

  Napi::Value GetWidth(const Napi::CallbackInfo& info);
  Napi::Value GetHeight(const Napi::CallbackInfo& info);
  Napi::Value GetData(const Napi::CallbackInfo& info);
  
  size_t width_;
  size_t height_;
  void* data_;
};
