#pragma once

#include <vector>
#include <napi.h>

using std::vector;

class TextMetrics : public Napi::ObjectWrap<TextMetrics> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value CreateInstance(Napi::Env env, vector<double> values);
    static Napi::FunctionReference constructor;

    TextMetrics(const Napi::CallbackInfo& info);

private:
    Napi::Value GetWidth(const Napi::CallbackInfo& info);
    Napi::Value GetActualBoundingBoxLeft(const Napi::CallbackInfo& info);
    Napi::Value GetActualBoundingBoxRight(const Napi::CallbackInfo& info);
    Napi::Value GetFontBoundingBoxAscent(const Napi::CallbackInfo& info);
    Napi::Value GetFontBoundingBoxDescent(const Napi::CallbackInfo& info);
    Napi::Value GetActualBoundingBoxAscent(const Napi::CallbackInfo& info);
    Napi::Value GetActualBoundingBoxDescent(const Napi::CallbackInfo& info);
    Napi::Value GetEmHeightAscent(const Napi::CallbackInfo& info);
    Napi::Value GetEmHeightDescent(const Napi::CallbackInfo& info);
    Napi::Value GetHangingBaseline(const Napi::CallbackInfo& info);
    Napi::Value GetAlphabeticBaseline(const Napi::CallbackInfo& info);
    Napi::Value GetIdeographicBaseline(const Napi::CallbackInfo& info);

    double width_;
    double actualBoundingBoxLeft_;
    double actualBoundingBoxRight_;
    double fontBoundingBoxAscent_;
    double fontBoundingBoxDescent_;
    double actualBoundingBoxAscent_;
    double actualBoundingBoxDescent_;
    double emHeightAscent_;
    double emHeightDescent_;
    double hangingBaseline_;
    double alphabeticBaseline_;
    double ideographicBaseline_;
};
