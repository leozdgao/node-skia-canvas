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
        DECLARE_NAPI_METHOD("getContext", GetContext),
        DECLARE_NAPI_METHOD("toBuffer", ToBuffer),
    };

    status = napi_define_class(env, "Canvas", NAPI_AUTO_LENGTH, New, nullptr, 2, properties, &cls);
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
    std::cout << "[] Call New" << std::endl;

    napi_status status;
    bool is_constructor = node_skia_helpers::is_called_by_new(env, info);

    std::cout << "[] isConstructor " << is_constructor << std::endl;

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

    }
}

void Canvas::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    std::cout << "[] Call destructor" << std::endl;
}

napi_value Canvas::GetContext(napi_env env, napi_callback_info info) {
    std::cout << "[] get context" << std::endl;
    napi_status status;
    size_t argc = 1;
    napi_value argv[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr);

    std::string cc = node_skia_helpers::get_utf8_string(env, argv[0]);

    std::cout << "[] get context with params " << cc << " length " << cc.size() << std::endl;

    Canvas* canvas;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&canvas));

    if (canvas->ctx_ == nullptr) {
        status = CanvasRenderingContext2D::NewInstance(env, &canvas->ctx_);
    }
    
    return canvas->ctx_;
}

napi_value Canvas::ToBuffer(napi_env env, napi_callback_info info) {
    std::cout << "[] to buffer" << std::endl;
}
