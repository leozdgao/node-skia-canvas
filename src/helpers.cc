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
    /**
     * check the constructor is alled by NEW
     */
    bool is_called_by_new(napi_env env, napi_callback_info info) {
        napi_value target;
        napi_status status = napi_get_new_target(env, info, &target);

        assert(status == napi_ok);

        return target != nullptr;
    }

    /**
     * get string from napi_value
     */
    string get_utf8_string(napi_env env, napi_value value) {
        // 首先读长度
        size_t str_len;
        napi_status status = napi_get_value_string_utf8(env, value, nullptr, 0, &str_len);

        // char buffer length is string length plus terminal char(\0)
        char buf[str_len + 1];
        status = napi_get_value_string_utf8(env, value, buf, str_len + 1, &str_len);

        assert(status == napi_ok);

        return string(buf);
    }

    /**
     * parse a funcion call expression
     */
    FunctionExpression parse_func_str(string &str) {
        FunctionExpression result;
        regex regx = regex(R"(^([^(]+)\((.+)\)$)");
        smatch sm;

        if (regex_match(str, sm, regx)) {
            result.name = sm[1];

            string params_str = sm[2];
            regex split_regx = regex(R"(\s*,\s*)");

            result.params = vector<string>(
                sregex_token_iterator(params_str.begin(), params_str.end(), split_regx, -1),
                sregex_token_iterator()
            );
        };

        return result;
    }
}
