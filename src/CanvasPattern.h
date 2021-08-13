#pragma once

#include <napi.h>
#include "include/core/SkTileMode.h"
#include "Canvas.h"
#include "Image.h"

class CanvasPattern: public Napi::ObjectWrap<CanvasPattern> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  CanvasPattern(const Napi::CallbackInfo& info);

  sk_sp<SkShader> toShader(bool smoothing);

private:
  SkTileMode repetition_[2];
  Image* img_ = nullptr;
  Canvas* canvas_ = nullptr;
};
