#pragma once

#include <napi.h>

#include "CanvasRenderingContext2D.h"
#include "include/core/SkSurface.h"

class Canvas : public Napi::ObjectWrap<Canvas> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::FunctionReference constructor;

    Canvas(const Napi::CallbackInfo& info);
    // static napi_value Init(napi_env env, napi_value exports);
    // static inline napi_value Constructor(napi_env env);
    // static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    sk_sp<SkSurface> getSurface();
    int getWidth();
    int getHeight();

private:
    Napi::Value GetWidth(const Napi::CallbackInfo& info);
    void SetWidth(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value GetHeight(const Napi::CallbackInfo& info);
    void SetHeight(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value GetContext(const Napi::CallbackInfo& info);
    Napi::Value ToBuffer(const Napi::CallbackInfo& info);

    napi_env env_;
    napi_ref wrapper_;

    int width_;
    int height_;
    sk_sp<SkSurface> rasterSurface_;

    CanvasRenderingContext2D* inner_ctx = nullptr;
};
