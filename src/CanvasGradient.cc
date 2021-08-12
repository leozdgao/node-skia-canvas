#include <algorithm>
#include <string>
#include "CanvasGradient.h"
#include "W3CSkColorParser.h"

CanvasGradient::CanvasGradient(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CanvasGradient>(info) {
    double start_x, start_y, end_x, end_y, r0, r1;

    if (info[0].IsString()) {
        Napi::String type = info[0].ToString();
        std::string type_str = type.Utf8Value();

        if (type_str == "linear") {
            gradient_type = GradientType::Linear;
        } else if (type_str == "radial") {
            gradient_type = GradientType::Radial;
        } else if (type_str == "conic") {
            gradient_type = GradientType::Conic;
        }
    };

    if (info[1].IsNumber()) {
        start_x = info[1].ToNumber().DoubleValue();
    }

    if (info[2].IsNumber()) {
        start_y = info[2].ToNumber().DoubleValue();
    }

    if (info[3].IsNumber()) {
        end_x = info[3].ToNumber().DoubleValue();
    }

    if (info[4].IsNumber()) {
        end_y = info[4].ToNumber().DoubleValue();
    }

    if (info[5].IsNumber()) {
        r0 = info[5].ToNumber().DoubleValue();
    }

    if (info[6].IsNumber()) {
        r1 = info[6].ToNumber().DoubleValue();
    }

    pts[0] = SkPoint::Make(start_x, start_y);
    pts[1] = SkPoint::Make(end_x, end_y);
    radius[0] = r0;
    radius[1] = r1;
}

Napi::FunctionReference CanvasGradient::constructor;

vector<SkScalar> CanvasGradient::getSortedGradientPos() {
    vector<SkScalar> col = {};
    for (auto i = this->colors.begin(); i != this->colors.end(); i++) {
        col.push_back(i->pos);
    }
    return col;
}

vector<SkColor> CanvasGradient::getSortedGradientColors() {
    vector<SkColor> col = {};
    for (auto i = this->colors.begin(); i != this->colors.end(); i++) {
        col.push_back(i->color.toSkColor());
    }
    return col;
}

// =================== Wrap Method ===================

Napi::Object CanvasGradient::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "CanvasGradient", {
    InstanceMethod<&CanvasGradient::AddColorStop>("addColorStop"),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CanvasGradient", func);

  return exports;
}

void CanvasGradient::AddColorStop(const Napi::CallbackInfo& info) {
    SkScalar p = 0;
    SkColor4f color;

    if (info[0].IsNumber()) {
        p = info[0].ToNumber().FloatValue();
    }

    if (info[1].IsString()) {
        std::string color_str = info[1].ToString().Utf8Value();
        color = W3CSkColorParser::rgba_from_string(color_str);
    }

    colors.push_back({ .pos = p, .color = color });

    std::sort(this->colors.begin(), this->colors.end(), [](GradientPosColor a, GradientPosColor b) -> bool {
        return a.pos < b.pos;
    });
};
