#include <iostream>
#include "Canvas.h"

Canvas::Canvas(int width, int height) {

}

Canvas::~Canvas() {
    napi_delete_reference(env_, wrapper_);
}

napi_value Canvas::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value cls;
    napi_property_descriptor properties[] = {
        { "getContext", 0, GetContext, 0, 0, 0, napi_default, 0 },
        { "toBuffer", 0, ToBuffer, 0, 0, 0, napi_default, 0 }
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
    napi_value target;
    status = napi_get_new_target(env, info, &target);
    assert(status == napi_ok);

    bool is_constructor = target != nullptr;

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
}

napi_value Canvas::ToBuffer(napi_env env, napi_callback_info info) {
    std::cout << "[] to buffer" << std::endl;
}
