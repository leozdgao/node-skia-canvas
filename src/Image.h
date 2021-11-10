#pragma once

#include <napi.h>
#include "include/core/SkImage.h"

class Image : public Napi::ObjectWrap<Image> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  Image(const Napi::CallbackInfo& info);

  sk_sp<SkImage> getImage();

private:
  Napi::Value GetWidth(const Napi::CallbackInfo& info);
  Napi::Value GetHeight(const Napi::CallbackInfo& info);
  Napi::Value GetSource(const Napi::CallbackInfo& info);
  void SetSource(const Napi::CallbackInfo& info, const Napi::Value& value);

  
  size_t width_;
  size_t height_;
  size_t len_;
  void* data_ = nullptr;

  sk_sp<SkImage> image_ = nullptr;
};
