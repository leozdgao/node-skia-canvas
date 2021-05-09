#include <napi.h>
#include "util.h"

namespace node_skia_helpers {
    // 判断调用是否是通过 new 来构造的
    bool is_called_by_new(napi_env env, napi_callback_info info) {
        napi_value target;
        napi_status status = napi_get_new_target(env, info, &target);

        assert(status == napi_ok);

        return target != nullptr;
    }
}