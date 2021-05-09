#pragma once

#include <napi.h>

class CanvasRenderingContext2D {
public:
    static napi_value Init(napi_env env, napi_value exports);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    static napi_value drawText(napi_env env, napi_callback_info info);

    napi_env env_;
    napi_ref wrapper_;
};

