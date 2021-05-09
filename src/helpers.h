#pragma once

#include <napi.h>

namespace node_skia_helpers {
    bool is_called_by_new(napi_env env, napi_callback_info info);
}
