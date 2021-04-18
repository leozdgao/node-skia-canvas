#pragma once

#include <napi.h>

class Canvas {
public:
    static napi_value Init(napi_env env, napi_value exports);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

private:
    explicit Canvas();
    ~Canvas();

    static inline napi_value Constructor(napi_env env);
    static napi_value New(napi_env env, napi_callback_info info);

    static napi_value GetContext(napi_env env, napi_callback_info info);
    static napi_value ToBuffer(napi_env env, napi_callback_info info);

    napi_env env_;
    napi_ref wrapper_;
};
