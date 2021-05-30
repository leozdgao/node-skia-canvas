#pragma once

#include <string>
#include <vector>
#include <napi.h>

using std::string;
using std::vector;

#define GET_CB_INFO_WITHOUT_ARG(env, info, status) \ 
    napi_value jsthis; \
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr); \

#define GET_CB_INFO(env, info, status, arg_count) \
    size_t argc = arg_count; \
    napi_value argv[arg_count]; \
    napi_value jsthis; \
    status = napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr); \

#define DECLARE_NAPI_METHOD(name, func)  { \
    name /* utf8name */, \
    0 /* name */, \
    func /* method */, \
    0 /* getter */, \
    0 /* setter */, \
    0 /* value */, \
    napi_default /* attributes */, \
    0 /* data */ \
}

#define DECLARE_NAPI_PROPERTY(name, getter, setter) { \
    name /* utf8name */, \
    0 /* name */, \
    0 /* method */, \
    getter /* getter */, \
    setter /* setter */, \
    0 /* value */, \
    napi_default /* attributes */, \
    0 /* data */ \
}

#define DECLARE_NAPI_VALUE(name, value) { \
    name /* utf8name */, \
    0 /* name */, \
    0 /* method */, \
    0 /* getter */, \
    0 /* setter */, \
    value /* value */, \
    napi_default /* attributes */, \
    0 /* data */ \
}

namespace node_skia_helpers {
    struct FunctionExpression {
        string name;
        vector<string> params;
    };

    bool is_called_by_new(napi_env env, napi_callback_info info);
    string get_utf8_string(napi_env env, napi_value value);

    FunctionExpression parse_func_str(string &str);
}
