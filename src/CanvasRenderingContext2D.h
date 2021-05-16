#pragma once

#include <napi.h>

class CanvasRenderingContext2D {
public:
    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    static napi_value FillText(napi_env env, napi_callback_info info);

    napi_env env_;
    napi_ref wrapper_;
};

