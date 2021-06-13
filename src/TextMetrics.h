#include <napi.h>

class TextMetrics {
public:
    TextMetrics(/* args */);
    ~TextMetrics();

    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    static napi_value GetWidth(napi_env env, napi_callback_info info);
    static napi_value GetActualBoundingBoxLeft(napi_env env, napi_callback_info info);
    static napi_value GetWidth(napi_env env, napi_callback_info info);
    static napi_value GetWidth(napi_env env, napi_callback_info info);
    static napi_value GetWidth(napi_env env, napi_callback_info info);

    float width;
    float actualBoundingBoxLeft;
    float actualBoundingBoxRight;
    float fontBoundingBoxAscent;
    float fontBoundingBoxDescent;
    float actualBoundingBoxAscent;
    float actualBoundingBoxDescent;
    float emHeightAscent;
    float emHeightDescent;
    float hangingBaseline;
    float alphabeticBaseline;
    float ideographicBaseline;

private:
    /* data */
};
