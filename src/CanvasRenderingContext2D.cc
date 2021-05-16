#include <iostream>

#include <include/core/SkTextBlob.h>

#include "CanvasRenderingContext2D.h"
#include "helpers.h"

CanvasRenderingContext2D::CanvasRenderingContext2D() {
    // 初始化 ctx 内部绘制状态
}

CanvasRenderingContext2D::~CanvasRenderingContext2D() {
    napi_delete_reference(env_, wrapper_);
}

napi_status CanvasRenderingContext2D::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_METHOD("fillText", FillText)
    };

    napi_value cons;
    status = napi_define_class(env, "CanvasRenderingContext2D", NAPI_AUTO_LENGTH, New, nullptr, 1, properties, &cons);
    assert(status == napi_ok);

    napi_ref* constructor = new napi_ref;
    status = napi_create_reference(env, cons, 1, constructor);
    assert(status == napi_ok);

    status = napi_set_instance_data(
      env,
      constructor,
      [](napi_env env, void* data, void* hint) {
        napi_ref* constructor = static_cast<napi_ref*>(data);
        napi_status status = napi_delete_reference(env, *constructor);
        assert(status == napi_ok);
        delete constructor;
      },
      nullptr
    );
    assert(status == napi_ok);

    return napi_ok;
}

/**
 * 获取构造函数
 */
napi_value CanvasRenderingContext2D::Constructor(napi_env env) {
    void* instance_data = nullptr;
    napi_status status = napi_get_instance_data(env, &instance_data);
    assert(status == napi_ok);
    napi_ref* constructor = static_cast<napi_ref*>(instance_data);

    napi_value cons;
    status = napi_get_reference_value(env, *constructor, &cons);
    assert(status == napi_ok);

    return cons;
}

/**
 * 实例构造逻辑
 */
napi_value CanvasRenderingContext2D::New(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 0;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, nullptr, &jsthis, nullptr);

    CanvasRenderingContext2D* ctx = new CanvasRenderingContext2D();
    ctx->env_ = env;
    status = napi_wrap(env, jsthis, reinterpret_cast<void*>(ctx), CanvasRenderingContext2D::Destructor, nullptr, &ctx->wrapper_);
    assert(status == napi_ok);

    return jsthis;
}

/**
 * 暴露给 JS 的工厂方法
 */
napi_status CanvasRenderingContext2D::NewInstance(napi_env env, napi_value* result) {
    napi_status status;
    status = napi_new_instance(env, Constructor(env), 0, nullptr, result);
    assert(status == napi_ok);

    return napi_ok;
}

void CanvasRenderingContext2D::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    reinterpret_cast<CanvasRenderingContext2D*>(nativeObject)->~CanvasRenderingContext2D();
}

void CanvasRenderingContext2D::SetCanvas(SkCanvas* canvas) {
    canvas_ = canvas;
};

napi_value CanvasRenderingContext2D::FillText(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value result;

    size_t argc = 1;
    napi_value argv[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr);

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    // string input = node_skia_helpers::get_utf8_string(env, argv[0]);
    auto text = SkTextBlob::MakeFromString("input.data()", SkFont(nullptr, 18));

    // std::cout << "get intput " << input << std::endl;

    SkPaint paint;
    ctx->canvas_->drawTextBlob(text.get(), 50, 25, paint);

    // return result;
}
