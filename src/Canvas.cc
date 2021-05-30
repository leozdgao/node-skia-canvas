#include <iostream>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

#include "Canvas.h"
#include "CanvasRenderingContext2D.h"
#include "helpers.h"

Canvas::Canvas(int width, int height) {
    width_ = width;
    height_ = height;

    rasterSurface_ = SkSurface::MakeRasterN32Premul(width_, height_);
}

Canvas::~Canvas() {
    napi_delete_reference(env_, wrapper_);
}

// Binding 初始化层，定义如何向上暴露 API
napi_value Canvas::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value cls;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_PROPERTY("width", GetWidth, SetWidth),
        DECLARE_NAPI_PROPERTY("height", GetHeight, SetHeight),
        DECLARE_NAPI_METHOD("getContext", GetContext),
        DECLARE_NAPI_METHOD("toBuffer", ToBuffer),
    };

    status = napi_define_class(env, "Canvas", NAPI_AUTO_LENGTH, New, nullptr, 4, properties, &cls);
    assert(status == napi_ok);

    napi_ref* constructor = new napi_ref;
    status = napi_create_reference(env, cls, 1, constructor);
    assert(status == napi_ok);

    // status = napi_set_instance_data(
    //     env,
    //     constructor,
    //     [](napi_env env, void* data, void* hint) {
    //         napi_ref* constructor = static_cast<napi_ref*>(data);
    //         napi_status status = napi_delete_reference(env, *constructor);
    //         assert(status == napi_ok);
    //         delete constructor;
    //     },
    //     nullptr
    // );
    // assert(status == napi_ok);

    status = napi_set_named_property(env, exports, "Canvas", cls);
    assert(status == napi_ok);

    return exports;
}

napi_value Canvas::New(napi_env env, napi_callback_info info) {
    napi_status status;
    bool is_constructor = node_skia_helpers::is_called_by_new(env, info);

    if (is_constructor) {
        // 通过 new 的方式调用
        size_t argc = 2;
        napi_value args[2];
        napi_value ctx;
        status = napi_get_cb_info(env, info, &argc, args, &ctx, nullptr);
        assert(status == napi_ok);

        int width;
        int height;

        napi_get_value_int32(env, args[0], &width);
        napi_get_value_int32(env, args[1], &height);

        Canvas* canvas = new Canvas(width, height);
        canvas->env_ = env;
        status = napi_wrap(env, ctx, reinterpret_cast<void*>(canvas), Canvas::Destructor, nullptr, &canvas->wrapper_);
        assert(status == napi_ok);

        return ctx;
    } else {
        // 也去支持工厂方式构造
    }
}

void Canvas::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    
}

napi_value Canvas::GetWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);

    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    napi_value result;
    status = napi_create_int32(env, canvas->width_, &result);

    return result;
}

napi_value Canvas::SetWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 1;
    napi_value argv[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr);

    int32_t width;
    status = napi_get_value_int32(env, argv[0], &width);
    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    canvas->width_ = width;
    canvas->rasterSurface_ = SkSurface::MakeRasterN32Premul(canvas->width_, canvas->height_);
    canvas->inner_ctx->SetCanvas(canvas->rasterSurface_->getCanvas());

    return nullptr;
}

napi_value Canvas::GetHeight(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);

    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    napi_value result;
    status = napi_create_int32(env, canvas->height_, &result);

    return result;
}

napi_value Canvas::SetHeight(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 1;
    napi_value argv[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr);

    int32_t height;
    status = napi_get_value_int32(env, argv[0], &height);
    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    canvas->height_ = height;
    canvas->rasterSurface_ = SkSurface::MakeRasterN32Premul(canvas->width_, canvas->height_);
    canvas->inner_ctx->SetCanvas(canvas->rasterSurface_->getCanvas());

    return nullptr;
}

napi_value Canvas::GetContext(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 1;
    napi_value argv[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr);

    std::string cc = node_skia_helpers::get_utf8_string(env, argv[0]);

    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    napi_valuetype ctx_type;
    status = napi_typeof(env, canvas->ctx_, &ctx_type);

    // NOTE: 这里判断 == nullptr 会有问题，后续挖一下为什么
    if (ctx_type != napi_object) {
        status = CanvasRenderingContext2D::NewInstance(env, &canvas->ctx_);

        CanvasRenderingContext2D* inner_ctx;
        status = napi_unwrap(env, canvas->ctx_, reinterpret_cast<void**>(&inner_ctx));
        canvas->inner_ctx = inner_ctx;
        inner_ctx->SetCanvas(canvas->rasterSurface_->getCanvas());
    }
    
    return canvas->ctx_;
}

napi_value Canvas::ToBuffer(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);

    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));
    SkCanvas* c = canvas->rasterSurface_->getCanvas();
    sk_sp<SkImage> img(canvas->rasterSurface_->makeImageSnapshot());
    sk_sp<SkData> png(img->encodeToData());

    napi_value buf;
    status = napi_create_buffer_copy(env, png->size(), png->data(), nullptr, &buf); // 为什么必须是 buffer copy？

    assert(status == napi_ok);

    return buf;
}
