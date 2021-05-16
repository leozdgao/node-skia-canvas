#pragma once

#include <string>
#include <napi.h>

using std::string;

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

namespace node_skia_helpers {
    bool is_called_by_new(napi_env env, napi_callback_info info);
    string get_utf8_string(napi_env env, napi_value value);
}