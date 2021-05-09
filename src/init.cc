#include <napi.h>
#include "Canvas.h"

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // exports.Set(Napi::String::New(env, "hello"), Napi::String::New(env, "world"));
  Canvas::Init(env, exports);

  return exports;
}

NODE_API_MODULE(hello, Init)
