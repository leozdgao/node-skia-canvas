#include <napi.h>
#include "Canvas.h"
#include "CanvasGradient.h"
#include "CanvasPattern.h"
#include "CanvasRenderingContext2D.h"
#include "FontManager.h"
#include "Image.h"
#include "ImageData.h"
#include "TextMetrics.h"

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Canvas::Init(env, exports);
  CanvasGradient::Init(env, exports);
  CanvasPattern::Init(env, exports);
  CanvasRenderingContext2D::Init(env, exports);
  FontManager::Init(env, exports);
  Image::Init(env, exports);
  ImageData::Init(env, exports);
  TextMetrics::Init(env, exports);

  return exports;
}

NODE_API_MODULE(hello, Init)
