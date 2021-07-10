#pragma once

#include <napi.h>

class Image : public Napi::ObjectWrap<Image> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Image(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;

  Napi::Value GetWidth(const Napi::CallbackInfo& info);
  Napi::Value GetHeight(const Napi::CallbackInfo& info);
  Napi::Value GetSource(const Napi::CallbackInfo& info);
  void SetSource(const Napi::CallbackInfo& info, const Napi::Value& value);

  
  size_t width_;
  size_t height_;
  size_t len_;
  void* data_;
};
