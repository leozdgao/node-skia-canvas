#pragma once

#include <napi.h>
#include <include/core/SkCanvas.h>
#include "include/core/SkSurface.h"

class CanvasRenderingContext2D {
public:
    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    void SetCanvas(SkCanvas* canvas);

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    static napi_value FillText(napi_env env, napi_callback_info info);
    static napi_value GetFillStyle(napi_env env, napi_callback_info info);
    static napi_value SetFillStyle(napi_env env, napi_callback_info info);

    napi_env env_;
    napi_ref wrapper_;

    SkCanvas* canvas_;
    SkPaint paint_;
};

