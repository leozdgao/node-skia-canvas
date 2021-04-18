#include "Canvas.h"

// class Canvas {
// public:
//     static napi_value Init(napi_env env, napi_value exports);

// private:
//     explicit Canvas();
//     ~Canvas();

//     static napi_value GetContext(napi_env env, napi_callback_info info);
//     static napi_value ToBuffer(napi_env env, napi_callback_info info);

//     napi_env env_;
//     napi_ref wrapper_;
// };

napi_value Canvas::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value obj;
    napi_property_descriptor properties[] = {
        { "getContext", 0, GetContext, 0, 0, 0, napi_default, 0 },
        { "toBuffer", 0, ToBuffer, 0, 0, 0, napi_default, 0 }
    };

    status = napi_define_class(env, "Canvas", NAPI_AUTO_LENGTH, New, nullptr, 2, properties, &obj);
    assert(status == napi_ok);

    napi_ref* constructor = new napi_ref;
    status = napi_create_reference(env, obj, 1, constructor);
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

    status = napi_set_named_property(env, exports, "Canvas", obj);
    assert(status == napi_ok);

    return exports;
}

napi_value Canvas::New(napi_env env, napi_callback_info info) {

}

napi_value Canvas::GetContext(napi_env env, napi_callback_info info) {
    
}

napi_value Canvas::ToBuffer(napi_env env, napi_callback_info info) {
    
}
