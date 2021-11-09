#include "TextMetrics.h"

Napi::Value TextMetrics::CreateInstance(Napi::Env env, vector<double> values) {
    Napi::Array array = Napi::Array::New(env, values.size());
    for (int i = 0, l = values.size(); i < l; i++) {
        array.Set(i, Napi::Number::New(env, values[i]));
    }

    return constructor.New({ array });
}

Napi::Object TextMetrics::CreateObject(Napi::Env env, vector<double> values, vector<TextLineMetrics> lines) {
    Napi::Object result = Napi::Object::New(env);

    result.Set("width", values[0]);
    result.Set("height", values[12]);
    result.Set("actualBoundingBoxLeft", values[1]);
    result.Set("actualBoundingBoxRight", values[2]);
    result.Set("actualBoundingBoxAscent", values[3]);
    result.Set("actualBoundingBoxDescent", values[4]);
    result.Set("fontBoundingBoxAscent", values[5]);
    result.Set("fontBoundingBoxDescent", values[6]);
    result.Set("emHeightAscent", values[7]);
    result.Set("emHeightDescent", values[8]);
    result.Set("alphabeticBaseline", values[9]);
    result.Set("hangingBaseline", values[10]);
    result.Set("ideographicBaseline", values[11]);

    Napi::Array jsLines = Napi::Array::New(env);

    for (int i = 0, l = lines.size(); i < l; i++) {
        TextLineMetrics line = lines[i];
        Napi::Object jsLineMetrics = Napi::Object::New(env);
        jsLineMetrics.Set("startIndex", line.startIndex);
        jsLineMetrics.Set("endIndex", line.endIndex);
        jsLineMetrics.Set("width", line.width);
        jsLineMetrics.Set("height", line.height);
        jsLineMetrics.Set("hasHardBreak", line.hasHardBreak);

        jsLines.Set(i, jsLineMetrics);
    }

    result.Set("lines", jsLines);

    return result;
}

TextMetrics::TextMetrics(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TextMetrics>(info) {
    Napi::Array arg0 = info[0].As<Napi::Array>();

    if (arg0.IsArray()) {
        Napi::Number width = arg0.Get((uint32_t) 0).As<Napi::Number>();
        if (width.IsNumber()) {
            width_ = width.DoubleValue();
        }
        
        Napi::Number actualBoundingBoxLeft = arg0.Get(1).As<Napi::Number>();
        if (actualBoundingBoxLeft.IsNumber()) {
            actualBoundingBoxLeft_ = actualBoundingBoxLeft.DoubleValue();
        }

        Napi::Number actualBoundingBoxRight = arg0.Get(2).As<Napi::Number>();
        if (actualBoundingBoxRight.IsNumber()) {
          actualBoundingBoxRight_ = actualBoundingBoxRight.DoubleValue();
        }

        Napi::Number actualBoundingBoxAscent = arg0.Get(3).As<Napi::Number>();
        if (actualBoundingBoxAscent.IsNumber()) {
           actualBoundingBoxAscent_ = actualBoundingBoxAscent.DoubleValue();
        }

        Napi::Number actualBoundingBoxDescent = arg0.Get(4).As<Napi::Number>();
        if (actualBoundingBoxDescent.IsNumber()) {
            actualBoundingBoxDescent_ = actualBoundingBoxDescent.DoubleValue();
        }

        Napi::Number fontBoundingBoxAscent = arg0.Get(5).As<Napi::Number>();
        if (fontBoundingBoxAscent.IsNumber()) {
           fontBoundingBoxAscent_ = fontBoundingBoxAscent.DoubleValue();
        }

        Napi::Number fontBoundingBoxDescent = arg0.Get(6).As<Napi::Number>();
        if (fontBoundingBoxDescent.IsNumber()) {
            fontBoundingBoxDescent_ = fontBoundingBoxDescent.DoubleValue();
        }

        Napi::Number emHeightAscent = arg0.Get(7).As<Napi::Number>();
        if (emHeightAscent.IsNumber()) {
           emHeightAscent_ = emHeightAscent.DoubleValue();
        }

        Napi::Number emHeightDescent = arg0.Get(8).As<Napi::Number>();
        if (emHeightDescent.IsNumber()) {
            emHeightDescent_ = emHeightDescent.DoubleValue();
        }

        Napi::Number alphabeticBaseline = arg0.Get(9).As<Napi::Number>();
        if (alphabeticBaseline.IsNumber()) {
            alphabeticBaseline_ = alphabeticBaseline.DoubleValue();
        }

        Napi::Number hangingBaseline = arg0.Get(10).As<Napi::Number>();
        if (hangingBaseline.IsNumber()) {
            hangingBaseline_ = hangingBaseline.DoubleValue();
        }

        Napi::Number ideographicBaseline = arg0.Get(11).As<Napi::Number>();
        if (ideographicBaseline.IsNumber()) {
            ideographicBaseline_ = ideographicBaseline.DoubleValue();
        }
    }
}

Napi::FunctionReference TextMetrics::constructor;

Napi::Object TextMetrics::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "TextMetrics", {
        InstanceAccessor<&TextMetrics::GetWidth>("width"),
        InstanceAccessor<&TextMetrics::GetActualBoundingBoxAscent>("actualBoundingBoxAscent"),
        InstanceAccessor<&TextMetrics::GetActualBoundingBoxDescent>("actualBoundingBoxDescent"),
        InstanceAccessor<&TextMetrics::GetActualBoundingBoxLeft>("actualBoundingBoxLeft"),
        InstanceAccessor<&TextMetrics::GetActualBoundingBoxRight>("actualBoundingBoxRight"),
        InstanceAccessor<&TextMetrics::GetFontBoundingBoxAscent>("fontBoundingBoxAscent"),
        InstanceAccessor<&TextMetrics::GetFontBoundingBoxDescent>("fontBoundingBoxDescent"),
        InstanceAccessor<&TextMetrics::GetEmHeightAscent>("emHeightAscent"),
        InstanceAccessor<&TextMetrics::GetEmHeightDescent>("emHeightDescent"),
        InstanceAccessor<&TextMetrics::GetAlphabeticBaseline>("alphabeticBaseline"),
        InstanceAccessor<&TextMetrics::GetHangingBaseline>("hangingBaseline"),
        InstanceAccessor<&TextMetrics::GetIdeographicBaseline>("ideographicBaseline"),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("TextMetrics", func);

    return exports;
}

Napi::Value TextMetrics::GetWidth(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), width_);
}

Napi::Value TextMetrics::GetActualBoundingBoxLeft(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), actualBoundingBoxLeft_);
}

Napi::Value TextMetrics::GetActualBoundingBoxRight(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), actualBoundingBoxRight_);
}

Napi::Value TextMetrics::GetActualBoundingBoxAscent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), actualBoundingBoxAscent_);
}

Napi::Value TextMetrics::GetActualBoundingBoxDescent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), actualBoundingBoxDescent_);
}

Napi::Value TextMetrics::GetFontBoundingBoxAscent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), fontBoundingBoxAscent_);
}

Napi::Value TextMetrics::GetFontBoundingBoxDescent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), fontBoundingBoxDescent_);
}

Napi::Value TextMetrics::GetEmHeightAscent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), emHeightAscent_);
}

Napi::Value TextMetrics::GetEmHeightDescent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), emHeightDescent_);
}

Napi::Value TextMetrics::GetAlphabeticBaseline(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), alphabeticBaseline_);
}

Napi::Value TextMetrics::GetHangingBaseline(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), hangingBaseline_);
}

Napi::Value TextMetrics::GetIdeographicBaseline(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), ideographicBaseline_);
}
