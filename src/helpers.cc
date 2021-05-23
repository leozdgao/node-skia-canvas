#include <string>
#include <regex>
#include <napi.h>
#include "helpers.h"

using std::string;
using std::smatch;
using std::sregex_token_iterator;
using std::regex;
using std::regex_match;

namespace node_skia_helpers {
    // 判断调用是否是通过 new 来调用构造函数的
    bool is_called_by_new(napi_env env, napi_callback_info info) {
        napi_value target;
        napi_status status = napi_get_new_target(env, info, &target);

        assert(status == napi_ok);

        return target != nullptr;
    }

    // 根据 napi_value 获取字符串
    string get_utf8_string(napi_env env, napi_value value) {
        // 首先读长度
        size_t str_len;
        napi_status status = napi_get_value_string_utf8(env, value, nullptr, 0, &str_len);

        // 将内容 copy 到对应内容空间，长度为字符串长度 + 1（for 字符串终止符）
        char buf[str_len + 1];
        status = napi_get_value_string_utf8(env, value, buf, str_len + 1, &str_len);

        assert(status == napi_ok);

        return string(buf);
    }

    FunctionExpression parse_func_str(string &str) {
        FunctionExpression result;
        regex regx = regex("^([^(]+)\\((.+)\\)$");
        smatch sm;
        string params_str;

        if (regex_match(str, sm, regx)) {
            result.name = sm[1];
            params_str = sm[2];

            regex split_regx = regex("\\s*,\\s*");

            result.params = vector<string>(
                sregex_token_iterator(params_str.begin(), params_str.end(), split_regx, -1),
                sregex_token_iterator()
            );
        };

        return result;
    }
}
