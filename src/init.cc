#include <napi.h>
#include "Canvas.h"
#include "CanvasRenderingContext2D.h"
#include "Image.h"

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Canvas::Init(env, exports);
  CanvasRenderingContext2D::Init(env, exports);
  Image::Init(env, exports);

  return exports;
}

NODE_API_MODULE(hello, Init)
