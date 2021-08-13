#pragma once

#include <vector>
#include <napi.h>
#include "include/core/SkColor.h"
#include "include/core/SkPoint.h"

using std::vector;

enum GradientType {
    Linear, Radial, Conic
};

struct GradientPosColor {
  SkScalar pos;
  SkColor4f color;
};

class CanvasGradient : public Napi::ObjectWrap<CanvasGradient> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  CanvasGradient(const Napi::CallbackInfo& info);

  vector<SkScalar> getSortedGradientPos();
  vector<SkColor> getSortedGradientColors();

  GradientType gradient_type = GradientType::Linear;
  SkPoint pts[2];
  SkScalar radius[2];
  vector<GradientPosColor> colors = {};

private:
  void AddColorStop(const Napi::CallbackInfo& info);
};
