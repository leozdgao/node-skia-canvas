#include <napi.h>

#include <string>

#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkColor.h"
#include "include/core/SkPath.h"
#include "include/core/SkPaint.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

using std::string;

void draw(SkCanvas* canvas) {
    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPath path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < 7; ++i) {
        SkScalar theta = 3 * i * TAU / 7;
        path.lineTo(R * cos(theta), R * sin(theta));
    }

    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    canvas->clear(SK_ColorWHITE);
    canvas->translate(0.5f * scale, 0.5f * scale);
    canvas->drawPath(path, p);
}


static Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  string msg;

  if (info.Length() < 1 || !info[0].IsString()) {
    msg = "world";
  } else {
    Napi::String val = info[0].ToString();
    msg = val.Utf8Value();
  }

  sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(300, 300);
  SkCanvas* rasterCanvas = rasterSurface->getCanvas();
  draw(rasterCanvas);

  sk_sp<SkImage> img(rasterSurface->makeImageSnapshot());

  if (img) {
    sk_sp<SkData> png(img->encodeToData());

    if (png) {
      SkFILEWStream out("aa.png");
      (void)out.write(png->data(), png->size());

      png->ref();
    }
  }

  return Napi::String::New(env, "Hello, " + msg + "!");
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init)


/**
 * - 什么是 NAPI
 *   - 背景
 *   - 方法调用 result, error
 * - 使用 NAPI 开发 Addon
 *   - 工具链（C++ & cmake-js & prebuild）
 *   - 初始化，向 JS 上下文暴露成员（属性，方法）
 *   - 数据交换
 *   - Callback
 *   - 异常（Error、Exception）
 *   - 异步任务 & Promise
 * - 高级
 *   - worker_thread 中使用
 *   - 引入其他库
 */

