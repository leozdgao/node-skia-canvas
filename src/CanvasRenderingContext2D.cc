#include <math.h>
#include <cassert>

#include <include/core/SkImageInfo.h>
#include <include/core/SkTextBlob.h>
#include <include/effects/SkDashPathEffect.h>
#include <include/effects/SkGradientShader.h>
#include <modules/skparagraph/include/FontCollection.h>
#include <modules/skparagraph/include/Paragraph.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>
#include <modules/skparagraph/src/ParagraphImpl.h>
#include <modules/skparagraph/src/Run.h>

#include "CanvasGradient.h"
#include "CanvasPattern.h"
#include "CanvasRenderingContext2D.h"
#include "FontManager.h"
#include "ImageData.h"
#include "StyleParser.h"
#include "TextMetrics.h"
#include "W3CSkColorParser.h"
#include "helpers.h"

using skia::textlayout::Decoration;
using skia::textlayout::FontCollection;
using skia::textlayout::ParagraphBuilder;
using skia::textlayout::Paragraph;
using skia::textlayout::ParagraphImpl;
using skia::textlayout::Run;
using skia::textlayout::LineMetrics;
using node_skia::StyleParser;

CanvasRenderingContext2D::CanvasRenderingContext2D() {
    init_canvas_state();
}

CanvasRenderingContext2D::~CanvasRenderingContext2D() {
    napi_delete_reference(env_, wrapper_);
}

void CanvasRenderingContext2D::init_canvas_state() {
    if (!states_.empty()) {
        // clear first
        stack<CanvasState>().swap(states_);
    }

    CanvasState init_state = CanvasState();
    init_state.paint_for_fill_ = SkPaint();
    init_state.paint_for_fill_.setStyle(SkPaint::kFill_Style);
    init_state.paint_for_fill_.setAntiAlias(true);

    init_state.paint_for_stroke_ = SkPaint();
    init_state.paint_for_stroke_.setStyle(SkPaint::kStroke_Style);
    init_state.paint_for_stroke_.setAntiAlias(true);
    init_state.paint_for_stroke_.setStrokeMiter(10.0);
    init_state.paint_for_stroke_.setStrokeWidth(1.0);

    init_state.pargf_style_ = ParagraphStyle();
    init_state.pargf_style_.turnHintingOff();

    init_state.text_style_ = TextStyle();
    init_state.text_style_.setFontSize(10);
    init_state.text_style_.setHeight(1.2);
    init_state.text_style_.setHeightOverride(true);
    init_state.text_style_.setHalfLeading(true);

    sk_sp<SkTypeface> face = SkTypeface::MakeFromName("sans-serif", SkFontStyle::Normal());
    init_state.text_style_.setTypeface(face);
    init_state.text_baseline_ = TextBaseline::Alphabetic;

    states_.push(init_state);
}

napi_status CanvasRenderingContext2D::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor properties[] = {
        // properties
        DECLARE_NAPI_PROPERTY("fillStyle", GetFillStyle, SetFillStyle),
        DECLARE_NAPI_PROPERTY("font", GetFont, SetFont),
        DECLARE_NAPI_PROPERTY("globalAlpha", GetGlobalAlpha, SetGlobalAlpha),
        DECLARE_NAPI_PROPERTY("imageSmoothingEnabled", GetImageSmoothingEnabled, SetImageSmoothingEnabled),
        DECLARE_NAPI_PROPERTY("lineCap", GetLineCap, SetLineCap),
        DECLARE_NAPI_PROPERTY("lineDashOffset", GetLineDashOffset, SetLineDashOffset),
        DECLARE_NAPI_PROPERTY("lineJoin", GetLineJoin, SetLineJoin),
        DECLARE_NAPI_PROPERTY("lineWidth", GetLineWidth, SetLineWidth),
        DECLARE_NAPI_PROPERTY("miterLimit", GetMiterLimit, SetMiterLimit),
        DECLARE_NAPI_PROPERTY("shadowBlur", GetShadowBlur, SetShadowBlur),
        DECLARE_NAPI_PROPERTY("shadowColor", GetShadowColor, SetShadowColor),
        DECLARE_NAPI_PROPERTY("shadowOffsetX", GetShadowOffsetX, SetShadowOffsetX),
        DECLARE_NAPI_PROPERTY("shadowOffsetY", GetShadowOffsetY, SetShadowOffsetY),
        DECLARE_NAPI_PROPERTY("strokeStyle", GetStrokeStyle, SetStrokeStyle),
        DECLARE_NAPI_PROPERTY("textAlign", GetTextAlign, SetTextAlign),
        DECLARE_NAPI_PROPERTY("textBaseline", GetTextBaseline, SetTextBaseline),
        DECLARE_NAPI_PROPERTY("textDecoration", GetTextDecoration, SetTextDecoration),
        // methods
        DECLARE_NAPI_METHOD("arc", Arc),
        DECLARE_NAPI_METHOD("arcTo", ArcTo),
        DECLARE_NAPI_METHOD("beginPath", BeginPath),
        DECLARE_NAPI_METHOD("bezierCurveTo", BezierCurveTo),
        DECLARE_NAPI_METHOD("createImageData", CreateImageData),
        DECLARE_NAPI_METHOD("clearRect", ClearRect),
        DECLARE_NAPI_METHOD("clip", Clip),
        DECLARE_NAPI_METHOD("closePath", ClosePath),
        DECLARE_NAPI_METHOD("drawImage", DrawImage),
        DECLARE_NAPI_METHOD("ellipse", Ellipse),
        DECLARE_NAPI_METHOD("fill", Fill),
        DECLARE_NAPI_METHOD("fillRect", FillRect),
        DECLARE_NAPI_METHOD("fillText", FillText),
        DECLARE_NAPI_METHOD("getImageData", GetImageData),
        DECLARE_NAPI_METHOD("getLineDash", GetLineDash),
        DECLARE_NAPI_METHOD("getTransform", GetTransform),
        DECLARE_NAPI_METHOD("lineTo", LineTo),
        DECLARE_NAPI_METHOD("moveTo", MoveTo),
        DECLARE_NAPI_METHOD("putImageData", PutImageData),
        DECLARE_NAPI_METHOD("quadraticCurveTo", QuadraticCurveTo),
        DECLARE_NAPI_METHOD("rect", Rect),
        DECLARE_NAPI_METHOD("resetTransform", ResetTransform),
        DECLARE_NAPI_METHOD("restore", Restore),
        DECLARE_NAPI_METHOD("rotate", Rotate),
        DECLARE_NAPI_METHOD("save", Save),
        DECLARE_NAPI_METHOD("scale", Scale),
        DECLARE_NAPI_METHOD("setLineDash", SetLineDash),
        DECLARE_NAPI_METHOD("setTransform", SetTransform),
        DECLARE_NAPI_METHOD("stroke", Stroke),
        DECLARE_NAPI_METHOD("strokeRect", StrokeRect),
        DECLARE_NAPI_METHOD("strokeText", StrokeText),
        DECLARE_NAPI_METHOD("measureText", MeasureText),
        DECLARE_NAPI_METHOD("transform", Transform),
        DECLARE_NAPI_METHOD("translate", Translate),
    };

    napi_value cons;
    status = napi_define_class(env, "CanvasRenderingContext2D", NAPI_AUTO_LENGTH, New, nullptr, 51, properties, &cons);
    assert(status == napi_ok);

    napi_ref* constructor = new napi_ref;
    status = napi_create_reference(env, cons, 1, constructor);
    assert(status == napi_ok);

    status = napi_set_instance_data(
      env,
      constructor,
      [](napi_env env, void* data, void* hint) {
        napi_ref* constructor = static_cast<napi_ref*>(data);
        napi_status status = napi_delete_reference(env, *constructor);
        assert(status == napi_ok);
        delete constructor;
      },
      nullptr
    );
    assert(status == napi_ok);

    return napi_ok;
}

/**
 * 获取构造函数
 */
napi_value CanvasRenderingContext2D::Constructor(napi_env env) {
    void* instance_data = nullptr;
    napi_status status = napi_get_instance_data(env, &instance_data);
    assert(status == napi_ok);
    napi_ref* constructor = static_cast<napi_ref*>(instance_data);

    napi_value cons;
    status = napi_get_reference_value(env, *constructor, &cons);
    assert(status == napi_ok);

    return cons;
}

/**
 * 实例构造逻辑
 */
napi_value CanvasRenderingContext2D::New(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 0;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, nullptr, &jsthis, nullptr);

    CanvasRenderingContext2D* ctx = new CanvasRenderingContext2D();
    ctx->env_ = env;
    status = napi_wrap(env, jsthis, reinterpret_cast<void*>(ctx), CanvasRenderingContext2D::Destructor, nullptr, &ctx->wrapper_);
    assert(status == napi_ok);

    return jsthis;
}

/**
 * 暴露给 JS 的工厂方法
 */
napi_status CanvasRenderingContext2D::NewInstance(napi_env env, napi_value* result) {
    napi_status status;
    status = napi_new_instance(env, Constructor(env), 0, nullptr, result);
    assert(status == napi_ok);

    return napi_ok;
}

void CanvasRenderingContext2D::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    reinterpret_cast<CanvasRenderingContext2D*>(nativeObject)->~CanvasRenderingContext2D();
}

void CanvasRenderingContext2D::setCanvas(SkCanvas* canvas) {
    canvas_ = canvas;
};
void CanvasRenderingContext2D::resetStates() {
    init_canvas_state();
}

// ================================== Properties ==================================

napi_value CanvasRenderingContext2D::GetFillStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->states_.top().paint_for_fill_.getColor4f();

    napi_value result, r, g, b, a;
    status = napi_create_array(env, &result);
    status = napi_create_double(env, color.fR, &r);
    status = napi_create_double(env, color.fG, &g);
    status = napi_create_double(env, color.fB, &b);
    status = napi_create_double(env, color.fA, &a);

    status = napi_set_element(env, result, 0, r);
    status = napi_set_element(env, result, 1, g);
    status = napi_set_element(env, result, 2, b);
    status = napi_set_element(env, result, 3, a);

    return result;
}

napi_value CanvasRenderingContext2D::SetFillStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->fill_with_dye(ctx->states_.top().paint_for_fill_, Napi::Value::From(env, argv[0]));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetFont(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    return Napi::String::New(env, ctx->states_.top().font_str);
}

napi_value CanvasRenderingContext2D::SetFont(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::Value arg0 = Napi::Value::From(env, argv[0]);
    
    if (arg0.IsObject()) {
        // Does not actually match fonts, just udpate style
        Napi::Object font_spec = arg0.As<Napi::Object>();
        Napi::String canonical = font_spec.Get("canonical").As<Napi::String>();
        ctx->states_.top().font_str = canonical.Utf8Value();

        Napi::String style = font_spec.Get("style").As<Napi::String>();
        Napi::String stretch = font_spec.Get("stretch").As<Napi::String>();
        Napi::Number weight = font_spec.Get("weight").As<Napi::Number>();
        Napi::Number size = font_spec.Get("size").As<Napi::Number>();
        Napi::Number leading = font_spec.Get("leading").As<Napi::Number>();
        Napi::Array families = font_spec.Get("family").As<Napi::Array>();
        vector<SkString> sk_families = {};

        for (int i = 0, len = families.Length(); i < len; i++) {
            Napi::Value item = families.Get(i);
            if (item.IsString()) {
                sk_families.push_back(SkString(item.As<Napi::String>().Utf8Value()));
            }
        }

        string s_slant = style.Utf8Value();
        string s_stretch = stretch.Utf8Value();
        ctx->states_.top().text_style_.setFontFamilies(sk_families);
        ctx->states_.top().text_style_.setFontStyle(
            SkFontStyle(weight.Int32Value(), StyleParser::fromStrToFontStrecth(s_stretch), StyleParser::fromStrToFontSlant(s_slant))
        );
        ctx->states_.top().text_style_.setFontSize(size.DoubleValue());
        ctx->states_.top().text_style_.setHeight(leading.DoubleValue() / size.DoubleValue());

        ctx->states_.top().text_style_.resetFontFeatures();
        Napi::Object features = font_spec.Get("features").As<Napi::Object>();
        Napi::Value on = features.Get("on");

        if (on.IsArray()) {
            Napi::Array a_features = on.As<Napi::Array>();

            for (int i = 0, l = a_features.Length(); i < l; i++) {
                Napi::Value item = a_features.Get(i);

                if (item.IsString()) {
                    ctx->states_.top().text_style_.addFontFeature(
                        SkString(item.As<Napi::String>().Utf8Value()),
                        1
                    );
                }
            }
        }
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetGlobalAlpha(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    status = napi_create_double(env, ctx->states_.top().global_alpha_, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetGlobalAlpha(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double alpha = 1;
    status = napi_get_value_double(env, argv[0], &alpha);
    
    ctx->states_.top().global_alpha_ = alpha;
    ctx->states_.top().paint_for_fill_.setAlphaf(alpha);
    ctx->states_.top().paint_for_stroke_.setAlphaf(alpha);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetImageSmoothingEnabled(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    bool smoothing = ctx->states_.top().image_smoothing_enable;

    return Napi::Boolean::New(env, smoothing);
}

napi_value CanvasRenderingContext2D::SetImageSmoothingEnabled(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    bool smoothing = false;
    status = napi_get_value_bool(env, argv[0], &smoothing);
    
    ctx->states_.top().image_smoothing_enable = smoothing;

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineCap(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    SkPaint::Cap cap = ctx->states_.top().paint_for_stroke_.getStrokeCap();
    string capStr = StyleParser::fromStrokeCapToStr(cap);
    
    return Napi::String::New(env, capStr);
}

napi_value CanvasRenderingContext2D::SetLineCap(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string cap = node_skia_helpers::get_utf8_string(env, argv[0]);
    ctx->states_.top().paint_for_stroke_.setStrokeCap(StyleParser::fromStrToStrokeCap(cap));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineDashOffset(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    double val = ctx->states_.top().line_dash_offset;
    
    return Napi::Number::New(env, val);
}

napi_value CanvasRenderingContext2D::SetLineDashOffset(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double line_dash_offset;
    status = napi_get_value_double(env, argv[0], &line_dash_offset);
    ctx->states_.top().line_dash_offset = line_dash_offset;

    sk_sp<SkPathEffect> effect = SkDashPathEffect::Make(ctx->states_.top().intervals.data(), 2, line_dash_offset);
    ctx->states_.top().paint_for_stroke_.setPathEffect(effect);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineJoin(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    SkPaint::Join join = ctx->states_.top().paint_for_stroke_.getStrokeJoin();
    string joinStr = StyleParser::fromStrokeJoinToStr(join);
    
    return Napi::String::New(env, joinStr);
}

napi_value CanvasRenderingContext2D::SetLineJoin(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string join = node_skia_helpers::get_utf8_string(env, argv[0]);
    ctx->states_.top().paint_for_stroke_.setStrokeJoin(StyleParser::fromStrToStrokeJoin(join));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().paint_for_stroke_.getStrokeWidth();
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetLineWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double line_width;
    status = napi_get_value_double(env, argv[0], &line_width);
    
    ctx->states_.top().paint_for_stroke_.setStrokeWidth(line_width);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetMiterLimit(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().paint_for_stroke_.getStrokeMiter();
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetMiterLimit(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double miter;
    status = napi_get_value_double(env, argv[0], &miter);
    
    ctx->states_.top().paint_for_stroke_.setStrokeMiter(miter);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetShadowBlur(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().shadow_blur;
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetShadowBlur(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double val;
    status = napi_get_value_double(env, argv[0], &val);
    
    ctx->states_.top().shadow_blur = val;

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetShadowColor(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->states_.top().shadow_color;

    napi_value result, r, g, b, a;
    status = napi_create_array(env, &result);
    status = napi_create_double(env, color.fR, &r);
    status = napi_create_double(env, color.fG, &g);
    status = napi_create_double(env, color.fB, &b);
    status = napi_create_double(env, color.fA, &a);

    status = napi_set_element(env, result, 0, r);
    status = napi_set_element(env, result, 1, g);
    status = napi_set_element(env, result, 2, b);
    status = napi_set_element(env, result, 3, a);

    return result;
}

napi_value CanvasRenderingContext2D::SetShadowColor(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string shadow_color_str = node_skia_helpers::get_utf8_string(env, argv[0]);
    shared_ptr<SkColor4f> shadow_color = W3CSkColorParser::rgba_from_string(shadow_color_str);

    if (shadow_color != nullptr) {
        CanvasRenderingContext2D* ctx;
        status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

        W3CSkColorParser::color_mix_with_alpha(*shadow_color, ctx->states_.top().global_alpha_);
        ctx->states_.top().shadow_color = *shadow_color;
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetShadowOffsetX(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().shadow_offset_x;
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetShadowOffsetX(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double val;
    status = napi_get_value_double(env, argv[0], &val);
    
    ctx->states_.top().shadow_offset_x = val;

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetShadowOffsetY(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().shadow_offset_y;
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetShadowOffsetY(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double val;
    status = napi_get_value_double(env, argv[0], &val);
    
    ctx->states_.top().shadow_offset_y = val;

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetStrokeStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->states_.top().paint_for_stroke_.getColor4f();

    napi_value result, r, g, b, a;
    status = napi_create_array(env, &result);
    status = napi_create_double(env, color.fR, &r);
    status = napi_create_double(env, color.fG, &g);
    status = napi_create_double(env, color.fB, &b);
    status = napi_create_double(env, color.fA, &a);

    status = napi_set_element(env, result, 0, r);
    status = napi_set_element(env, result, 1, g);
    status = napi_set_element(env, result, 2, b);
    status = napi_set_element(env, result, 3, a);

    return result;
}

napi_value CanvasRenderingContext2D::SetStrokeStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->fill_with_dye(ctx->states_.top().paint_for_stroke_, Napi::Value::From(env, argv[0]));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextAlign(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextAlign align = ctx->states_.top().pargf_style_.getTextAlign();
    string align_name = StyleParser::fromTextAlignToStr(align);

    napi_value result;
    napi_create_string_utf8(env, align_name.c_str(), align_name.size(), &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetTextAlign(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string text_align = node_skia_helpers::get_utf8_string(env, argv[0]);
    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    ctx->states_.top().pargf_style_.setTextAlign(
        StyleParser::fromStrToTextAlign(text_align, ctx->states_.top().pargf_style_.getTextAlign())
    );

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextBaseline(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextBaseline baseline = ctx->states_.top().text_baseline_;
    string baseline_name = StyleParser::fromTextBaselineToStr(baseline);

    napi_value result;
    napi_create_string_utf8(env, baseline_name.c_str(), baseline_name.size(), &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetTextBaseline(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string text_baseline = node_skia_helpers::get_utf8_string(env, argv[0]);
    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    ctx->states_.top().text_baseline_ = StyleParser::fromStrToTextBaseline(text_baseline);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextDecoration(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    CanvasState state = ctx->states_.top();
    Decoration decoration = state.text_style_.getDecoration();
    Napi::Array result = Napi::Array::New(env);

    result.Set((uint32_t)0, StyleParser::fromTextDecorationStyleToStr(decoration.fStyle));
    result.Set(1, StyleParser::fromTextDecorationTypeToStr(decoration.fType));

    Napi::Array color_arr = Napi::Array::New(env);
    SkColor4f color = SkColor4f::FromColor(decoration.fColor);
    color_arr.Set((uint32_t)0, color.fR);
    color_arr.Set(1, color.fG);
    color_arr.Set(2, color.fB);
    color_arr.Set(3, color.fA);

    result.Set(2, color_arr);
    result.Set(3, decoration.fThicknessMultiplier);

    return result;
}

napi_value CanvasRenderingContext2D::SetTextDecoration(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)
    
    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    Napi::Array values = Napi::Value::From(env, argv[0]).As<Napi::Array>();
    Napi::String type = values.Get((uint32_t)0).As<Napi::String>();
    Napi::String style = values.Get(1).As<Napi::String>();
    Napi::String color = values.Get(2).As<Napi::String>();
    Napi::Number thickness = values.Get(3).As<Napi::Number>();

    string type_str = type.Utf8Value();
    string style_str = style.Utf8Value();
    string color_str = color.Utf8Value();
    shared_ptr<SkColor4f> sk_color = W3CSkColorParser::rgba_from_string(color_str);

    ctx->states_.top().text_style_.setDecorationStyle(StyleParser::fromStrToTextDecorationStyle(style_str));
    ctx->states_.top().text_style_.setDecoration(StyleParser::fromStrToTextDecorationType(type_str));
    ctx->states_.top().text_style_.setDecorationColor(sk_color->toSkColor());
    ctx->states_.top().text_style_.setDecorationThicknessMultiplier(thickness.FloatValue());

    return nullptr;
}

// ================================== Methods ==================================

napi_value CanvasRenderingContext2D::Arc(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 5)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, radius, start_angle, end_angle;

    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &radius);
    status = napi_get_value_double(env, argv[3], &start_angle);
    status = napi_get_value_double(env, argv[4], &end_angle);

    double ax = x - radius;
    double ay = y - radius;

    SkRect oval = SkRect::MakeXYWH(ax, ay, radius * 2, radius * 2);
    SkPoint move_point = SkPoint::Make(x, y);

    if (ctx->states_.top().last_move_point_ && move_point != *ctx->states_.top().last_move_point_) {
        ctx->states_.top().path_.lineTo(move_point);
    }

    ctx->states_.top().path_.moveTo(move_point);
    ctx->states_.top().last_move_point_ = &move_point;

    SkScalar sweep_angle = end_angle - start_angle;

    if (sweep_angle == 360) {
      sweep_angle -= 1;
    }

    ctx->states_.top().path_.arcTo(oval, start_angle, sweep_angle, true);

    return nullptr;
}

napi_value CanvasRenderingContext2D::ArcTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 5)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x1, y1, x2, y2, radius;

    status = napi_get_value_double(env, argv[0], &x1);
    status = napi_get_value_double(env, argv[1], &y1);
    status = napi_get_value_double(env, argv[2], &x2);
    status = napi_get_value_double(env, argv[3], &y2);
    status = napi_get_value_double(env, argv[4], &radius);

    ctx->states_.top().path_.arcTo(SkPoint::Make(x1, y1), SkPoint::Make(x2, y2), radius);

    return nullptr;
}

napi_value CanvasRenderingContext2D::BeginPath(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->states_.top().path_.reset();
    ctx->states_.top().last_move_point_ = nullptr;

    return nullptr;
}

napi_value CanvasRenderingContext2D::BezierCurveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 6)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double cp1x, cp1y, cp2x, cp2y, x, y;

    status = napi_get_value_double(env, argv[0], &cp1x);
    status = napi_get_value_double(env, argv[1], &cp1y);
    status = napi_get_value_double(env, argv[2], &cp2x);
    status = napi_get_value_double(env, argv[3], &cp2y);
    status = napi_get_value_double(env, argv[4], &x);
    status = napi_get_value_double(env, argv[5], &y);

    ctx->states_.top().path_.cubicTo(SkPoint::Make(cp1x, cp1y), SkPoint::Make(cp2x, cp2y), SkPoint::Make(x, y));

    return nullptr;
}

napi_value CanvasRenderingContext2D::CreateImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    int width, height;

    status = napi_get_value_int32(env, argv[0], &width);
    status = napi_get_value_int32(env, argv[1], &height);

    SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(width, height), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    sk_sp<SkData> data(SkData::MakeUninitialized(image_info.minRowBytes() * image_info.height()));
    sk_bzero(data->writable_data(), image_info.minRowBytes() * image_info.height());

    return ImageData::CreateInstance(env, width, height, data->writable_data());
}

napi_value CanvasRenderingContext2D::ClearRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);
    SkPaint p = SkPaint();
    p.setBlendMode(SkBlendMode::kClear);

    ctx->canvas_->drawRect(rect, p);

    return nullptr;
}

napi_value CanvasRenderingContext2D::Clip(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkPath clip_path = ctx->states_.top().path_;

    Napi::String str = Napi::Value::From(env, argv[0]).As<Napi::String>();
    if (str.IsString() && str.Utf8Value() == "evenodd") {
        clip_path.setFillType(SkPathFillType::kEvenOdd);
    } else {
        clip_path.setFillType(SkPathFillType::kWinding);
    }
    
    ctx->canvas_->clipPath(clip_path, SkClipOp::kIntersect, true);

    return nullptr;
}

napi_value CanvasRenderingContext2D::ClosePath(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->states_.top().path_.close();

    return nullptr;
}

napi_value CanvasRenderingContext2D::DrawImage(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 9)
    // img, sx, sy, sW, sH, dx, dy, dW, dH

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double sx, sy, sW, sH, dx, dy, dW, dH;
    void* data;
    size_t len;

    status = napi_get_buffer_info(env, argv[0], &data, &len);
    status = napi_get_value_double(env, argv[1], &sx);
    status = napi_get_value_double(env, argv[2], &sy);
    status = napi_get_value_double(env, argv[3], &sW);
    status = napi_get_value_double(env, argv[4], &sH);
    status = napi_get_value_double(env, argv[5], &dx);
    status = napi_get_value_double(env, argv[6], &dy);
    status = napi_get_value_double(env, argv[7], &dW);
    status = napi_get_value_double(env, argv[8], &dH);

    sk_sp<SkImage> img = SkImage::MakeFromEncoded(
        SkData::MakeWithoutCopy(data, len)
    );
    SkRect src = SkRect();
    src.setXYWH(sx, sy, sW, sH);
    SkRect dist = SkRect();
    dist.setXYWH(dx, dy, dW, dH);

    bool smoothing = ctx->states_.top().image_smoothing_enable;
    SkSamplingOptions sample_options = smoothing ? SkSamplingOptions(SkFilterQuality::kHigh_SkFilterQuality) : SkSamplingOptions();

    ctx->render_to_canvas(ctx->states_.top().paint_for_fill_, [&](SkPaint& paint) {
        ctx->canvas_->drawImageRect(img, src, dist, sample_options, &paint, SkCanvas::kFast_SrcRectConstraint);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::Ellipse(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 8)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x = Napi::Value::From(env, argv[0]).As<Napi::Number>().DoubleValue();
    double y = Napi::Value::From(env, argv[1]).As<Napi::Number>().DoubleValue();
    double rx = Napi::Value::From(env, argv[2]).As<Napi::Number>().DoubleValue();
    double ry = Napi::Value::From(env, argv[3]).As<Napi::Number>().DoubleValue();
    double rotation = Napi::Value::From(env, argv[4]).As<Napi::Number>().DoubleValue();
    double start_angle = Napi::Value::From(env, argv[5]).As<Napi::Number>().DoubleValue();
    double end_angle = Napi::Value::From(env, argv[6]).As<Napi::Number>().DoubleValue();
    bool ccw = Napi::Value::From(env, argv[7]).As<Napi::Boolean>().Value();

    SkRect rect = SkRect::MakeXYWH(x - rx, y - ry, 2 * rx, 2 * ry);
    SkMatrix matrix = SkMatrix().preTranslate(x, y).preRotate(rotation / M_PI * 180).preTranslate(-x, -y);
    SkMatrix inverted_matrix;
    matrix.invert(&inverted_matrix);

    ctx->states_.top().path_.transform(inverted_matrix);

    double sweep_deg = (end_angle - start_angle) / M_PI * 180;
    double start_deg = start_angle / M_PI * 180;

    if (abs(sweep_deg - 360.0) < 0.00001) {
        double half_sweep = sweep_deg / 2.0;
        ctx->states_.top().path_.arcTo(rect, start_deg, half_sweep, false);
        ctx->states_.top().path_.arcTo(rect, start_deg + half_sweep, half_sweep, false);
    } else {
        ctx->states_.top().path_.arcTo(rect, start_deg, sweep_deg, false);
    }


    ctx->states_.top().path_.transform(matrix);

    return nullptr;
}

napi_value CanvasRenderingContext2D::Fill(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->render_to_canvas(ctx->states_.top().paint_for_fill_, [&](SkPaint& paint) {
        SkPath fill_path = ctx->states_.top().path_;

        Napi::String str = Napi::Value::From(env, argv[0]).As<Napi::String>();
        if (str.IsString() && str.Utf8Value() == "evenodd") {
            fill_path.setFillType(SkPathFillType::kEvenOdd);
        } else {
            fill_path.setFillType(SkPathFillType::kWinding);
        }
        ctx->canvas_->drawPath(fill_path, paint);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::FillRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);

    CanvasState state = ctx->states_.top();
    ctx->render_to_canvas(state.paint_for_fill_, [&](SkPaint& paint) {
        ctx->canvas_->drawRect(rect, paint);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::FillText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::String text = Napi::Value::From(env, argv[0]).As<Napi::String>();
    Napi::Number x = Napi::Value::From(env, argv[1]).As<Napi::Number>();
    Napi::Number y = Napi::Value::From(env, argv[2]).As<Napi::Number>();
    Napi::Number maxWidth = Napi::Value::From(env, argv[3]).As<Napi::Number>();
    SkScalar s_maxWidth = maxWidth.IsNumber() ? maxWidth.DoubleValue() : 10 * 10000;

    CanvasState state = ctx->states_.top();
    ctx->render_to_canvas(state.paint_for_fill_, [&](SkPaint& paint) {
        ctx->render_text(paint, text.Utf8Value(), x.DoubleValue(), y.DoubleValue(), s_maxWidth);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    int sx, sy, sw, sh;
    status = napi_get_value_int32(env, argv[0], &sx);
    status = napi_get_value_int32(env, argv[1], &sy);
    status = napi_get_value_int32(env, argv[2], &sw);
    status = napi_get_value_int32(env, argv[3], &sh);

    SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(sw, sh), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    sk_sp<SkData> data(SkData::MakeUninitialized(image_info.minRowBytes() * image_info.height()));
    sk_bzero(data->writable_data(), image_info.minRowBytes() * image_info.height());
    ctx->canvas_->readPixels(image_info, data->writable_data(), image_info.minRowBytes(), sx, sy);

    return ImageData::CreateInstance(env, image_info.width(), image_info.height(), data->writable_data());
}

napi_value CanvasRenderingContext2D::GetLineDash(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    vector<SkScalar> intervals = ctx->states_.top().intervals;
    Napi::Array array = Napi::Array::New(env, intervals.size());

    for (int i = 0, l = intervals.size(); i < l; i++) {
        array.Set(i, intervals[i]);
    }

    return array;
}

napi_value CanvasRenderingContext2D::GetTransform(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::Array array = Napi::Array::New(env, 6);
    SkMatrix matrix = ctx->canvas_->getTotalMatrix();

    array[(uint32_t)0] = matrix.getScaleX();
    array[2] = matrix.getSkewX();
    array[4] = matrix.getTranslateX();
    array[1] = matrix.getSkewY();
    array[3] = matrix.getScaleY();
    array[5] = matrix.getTranslateY();

    return array;
}

napi_value CanvasRenderingContext2D::LineTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    if (ctx->states_.top().last_move_point_ != nullptr) {
        double x, y;
        status = napi_get_value_double(env, argv[0], &x);
        status = napi_get_value_double(env, argv[1], &y);

        ctx->states_.top().path_.lineTo(SkPoint::Make(x, y));
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::MoveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);

    SkPoint move_point = SkPoint::Make(x, y);
    ctx->states_.top().last_move_point_ = &move_point;
    ctx->states_.top().path_.moveTo(move_point);

    return nullptr;
}

napi_value CanvasRenderingContext2D::PutImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 3)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    int dx, dy;
    ImageData* imgData;

    Napi::Object data = Napi::Value::From(env, argv[0]).As<Napi::Object>();
    Napi::Value dataValue = data.Get("data");

    if (dataValue.IsTypedArray()) {
        status = napi_unwrap(env, argv[0], reinterpret_cast<void**>(&imgData));
        status = napi_get_value_int32(env, argv[1], &dx);
        status = napi_get_value_int32(env, argv[2], &dy);

        SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(imgData->getWidth(), imgData->getHeight()), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
        ctx->canvas_->writePixels(image_info, dataValue.As<Napi::Uint8Array>().Data(), image_info.minRowBytes(), dx, dy);
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::QuadraticCurveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    double cpx, cpy, x, y;
    status = napi_get_value_double(env, argv[0], &cpx);
    status = napi_get_value_double(env, argv[1], &cpy);
    status = napi_get_value_double(env, argv[2], &x);
    status = napi_get_value_double(env, argv[3], &y);

    ctx->states_.top().path_.quadTo(SkPoint::Make(cpx, cpy), SkPoint::Make(x, y));

    return nullptr;
}

napi_value CanvasRenderingContext2D::Rect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkPoint move_point = SkPoint::Make(x, y);

    if (ctx->states_.top().last_move_point_ && move_point != *ctx->states_.top().last_move_point_) {
        ctx->states_.top().path_.lineTo(move_point);
    }

    ctx->states_.top().path_.moveTo(move_point);
    ctx->states_.top().last_move_point_ = &move_point;

    ctx->states_.top().path_.addRect(SkRect::MakeXYWH(x, y, w, h));

    return nullptr;
}

napi_value CanvasRenderingContext2D::ResetTransform(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->states_.top().matrix_ = SkMatrix();
    ctx->canvas_->resetMatrix();

    return nullptr;
}

napi_value CanvasRenderingContext2D::Restore(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    if (ctx->states_.size() > 1) {
        ctx->states_.pop();
        ctx->canvas_->restore();
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::Rotate(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::Value arg0 = Napi::Value::From(env, argv[0]);

    if (arg0.IsNumber()) {
        double degree = arg0.As<Napi::Number>().DoubleValue();

        if (!std::isnan(degree) && !std::isinf(degree)) {
            SkMatrix matrix = ctx->states_.top().matrix_;
            matrix = matrix.preRotate(degree / M_PI * 180);
            ctx->canvas_->setMatrix(matrix);
            ctx->states_.top().matrix_ = matrix;
        }
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::Save(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    // 所谓的保持状态，即是 Copy 当前状态并压栈
    
    CanvasState new_state = ctx->states_.top();
    ctx->states_.push(new_state);
    ctx->canvas_->save();

    return nullptr;
}

napi_value CanvasRenderingContext2D::Scale(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);

    SkMatrix matrix = ctx->states_.top().matrix_;
    matrix = matrix.preScale(x, y);
    ctx->canvas_->setMatrix(matrix);
    ctx->states_.top().matrix_ = matrix;

    return nullptr;
}

napi_value CanvasRenderingContext2D::SetLineDash(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::Array array = Napi::Array(env, argv[0]);
    int len = array.Length();
    vector<SkScalar> intervals = {};
    for (int i = 0; i < len; i++) {
        float val = array.Get(i).As<Napi::Number>().FloatValue();
        intervals.push_back(val);
    }

    ctx->states_.top().intervals = intervals;

    sk_sp<SkPathEffect> effect = SkDashPathEffect::Make(
        ctx->states_.top().intervals.data(),
        intervals.size(),
        ctx->states_.top().line_dash_offset
    );
    ctx->states_.top().paint_for_stroke_.setPathEffect(effect);

    return nullptr;
}

napi_value CanvasRenderingContext2D::SetTransform(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 6)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double a, b, c, d, e, f;

    status = napi_get_value_double(env, argv[0], &a);
    status = napi_get_value_double(env, argv[1], &b);
    status = napi_get_value_double(env, argv[2], &c);
    status = napi_get_value_double(env, argv[3], &d);
    status = napi_get_value_double(env, argv[4], &e);
    status = napi_get_value_double(env, argv[5], &f);

    CanvasState state = ctx->states_.top();
    state.matrix_ = state.matrix_.setAll(a, c, e, b, d, f, 0, 0, 1);
    ctx->canvas_->setMatrix(state.matrix_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::Stroke(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    ctx->render_to_canvas(ctx->states_.top().paint_for_stroke_, [&](SkPaint& paint) {
        ctx->canvas_->drawPath(ctx->states_.top().path_, paint);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::StrokeRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);

    ctx->render_to_canvas(ctx->states_.top().paint_for_stroke_, [&](SkPaint& paint) {
        ctx->canvas_->drawRect(rect, paint);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::StrokeText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::String text = Napi::Value::From(env, argv[0]).As<Napi::String>();
    Napi::Number x = Napi::Value::From(env, argv[1]).As<Napi::Number>();
    Napi::Number y = Napi::Value::From(env, argv[2]).As<Napi::Number>();
    Napi::Number maxWidth = Napi::Value::From(env, argv[3]).As<Napi::Number>();
    SkScalar s_maxWidth = maxWidth.IsNumber() ? maxWidth.DoubleValue() : 10 * 10000;

    CanvasState state = ctx->states_.top();
    ctx->render_to_canvas(state.paint_for_stroke_, [&](SkPaint& paint) {
        ctx->render_text(paint, text.Utf8Value(), x.DoubleValue(), y.DoubleValue(), s_maxWidth);
    });

    return nullptr;
}

napi_value CanvasRenderingContext2D::MeasureText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::String text = Napi::Value::From(env, argv[0]).As<Napi::String>();
    Napi::Number maxWidth = Napi::Value::From(env, argv[1]).As<Napi::Number>();
    SkScalar s_maxWidth = maxWidth.IsNumber() ? maxWidth.DoubleValue() : 10 * 10000;

    vector<TextLineMetrics> lines = {};
    vector<double> values = ctx->measure_text(ctx->states_.top().paint_for_fill_, text.Utf8Value(), s_maxWidth, &lines);

    return TextMetrics::CreateObject(env, values, lines);
}

napi_value CanvasRenderingContext2D::Transform(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 6)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double a, b, c, d, e, f;

    status = napi_get_value_double(env, argv[0], &a);
    status = napi_get_value_double(env, argv[1], &b);
    status = napi_get_value_double(env, argv[2], &c);
    status = napi_get_value_double(env, argv[3], &d);
    status = napi_get_value_double(env, argv[4], &e);
    status = napi_get_value_double(env, argv[5], &f);

    SkMatrix new_matrix = SkMatrix::MakeAll(a, c, e, b, d, f, 0, 0, 1);
    SkMatrix matrix = ctx->states_.top().matrix_;
    matrix = matrix.preConcat(new_matrix);
    ctx->canvas_->setMatrix(matrix);
    ctx->states_.top().matrix_ = matrix;

    return nullptr;
}

napi_value CanvasRenderingContext2D::Translate(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);

    SkMatrix matrix = ctx->states_.top().matrix_;
    matrix = matrix.preTranslate(x, y);
    ctx->canvas_->setMatrix(matrix);
    ctx->states_.top().matrix_ = matrix;

    return nullptr;
}

// ======================= Private =======================

vector<double> CanvasRenderingContext2D::measure_text(SkPaint& paint, string text, SkScalar maxWidth, vector<TextLineMetrics>* lines) {
    TextStyle text_style = this->states_.top().text_style_;
    text_style.setForegroundColor(paint);

    ParagraphStyle pragh_style = this->states_.top().pargf_style_;
    std::unique_ptr<ParagraphBuilder> builder = ParagraphBuilder::make(pragh_style, FontManager::collection.getCollection());
    builder->pushStyle(text_style);
    builder->addText(text.data());

    ParagraphImpl* paragraph = static_cast<ParagraphImpl*>(builder->Build().release());
    paragraph->layout(maxWidth);

    Run run = paragraph->run(0);
    int run_size = run.size();
    SkRect first_char_bounds = run.getBounds(0);
    SkScalar actual_ascent = first_char_bounds.fTop;
    SkScalar actual_descent = first_char_bounds.fBottom;

    for (int i = 1; i < run_size; i++) {
        SkRect char_bounds = run.getBounds(i);
        SkScalar top = char_bounds.fTop;
        SkScalar bottom = char_bounds.fBottom;

        if (top < actual_ascent) {
            actual_ascent = top;
        }

        if (bottom > actual_descent) {
            actual_descent = bottom;
        }
    }

    SkFontMetrics metrics;
    text_style.getFontMetrics(&metrics);
    SkScalar offset = StyleParser::getTextBaselineOffset(metrics, this->states_.top().text_baseline_);
    SkScalar hang = StyleParser::getTextBaselineOffset(metrics, TextBaseline::Hanging) - offset;
    SkScalar norm = StyleParser::getTextBaselineOffset(metrics, TextBaseline::Alphabetic) - offset;
    SkScalar ideo = StyleParser::getTextBaselineOffset(metrics, TextBaseline::Ideographic) - offset;
    SkScalar ascent = norm - metrics.fAscent;
    SkScalar descent = metrics.fDescent - norm;

    vector<double> result = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    if (paragraph->lineNumber() == 0) {
        return result;
    }

    vector<LineMetrics> line_metrics;
    paragraph->getLineMetrics(line_metrics);

    LineMetrics first_line = line_metrics[0];
    double width = 0.0;
    double height = 0.0;

    for (LineMetrics metrics: line_metrics) {
        width = std::max(width, metrics.fWidth);
        height += metrics.fHeight;

        if (lines != nullptr) {
            TextLineMetrics line = {
                metrics.fStartIndex, metrics.fEndIndex, metrics.fWidth, metrics.fHeight, metrics.fHardBreak
            };
            lines->push_back(line);
        }
    }

    // width
    result[0] = width;
    // actualBoundingBoxLeft
    result[1] = StyleParser::getTextAlignFactor(pragh_style) * first_line.fWidth;
    // actualBoundingBoxRight
    result[2] = first_line.fWidth + StyleParser::getTextAlignFactor(pragh_style) * first_line.fWidth;
    // actualBoundingBoxAscent
    result[3] = ascent;
    // actualBoundingBoxDescent
    result[4] = descent;
    // fontBoundingBoxAscent
    result[5] = SkScalarRoundToScalar(norm + first_line.fAscent);
    // fontBoundingBoxDescent
    result[6] = SkScalarRoundToScalar(first_line.fDescent - norm);
    // emHeightAscent
    result[7] = SkScalarRoundToScalar(norm + first_line.fAscent);
    // emHeightDescent
    result[8] = SkScalarRoundToScalar(first_line.fDescent - norm);
    // alphabeticBaseline
    result[9] = norm;
    // hangingBaseline
    result[10] = hang;
    // ideographicBaseline
    result[11] = ideo;
    // height
    result[12] = height;

    return result;
}

void CanvasRenderingContext2D::render_text(SkPaint& paint, string text, SkScalar x, SkScalar y, SkScalar maxWidth) {
    TextStyle text_style = this->states_.top().text_style_;
    text_style.setForegroundColor(paint);

    ParagraphStyle pragh_style = this->states_.top().pargf_style_;
    std::unique_ptr<ParagraphBuilder> builder = ParagraphBuilder::make(pragh_style, FontManager::collection.getCollection());
    builder->pushStyle(text_style);
    builder->addText(text.data());

    std::unique_ptr<Paragraph> paragraph = builder->Build();
    paragraph->layout(maxWidth);

    SkFontMetrics metrics;
    text_style.getFontMetrics(&metrics);
    SkScalar offset_x = maxWidth * StyleParser::getTextAlignFactor(pragh_style);
    SkScalar offset_y = StyleParser::getTextBaselineOffset(metrics, this->states_.top().text_baseline_);

    paragraph->paint(this->canvas_, x + offset_x, y + offset_y);

    // vector<sk_sp<SkTypeface>> typefaces = FontManager::collection->findTypefaces(text_style.getFontFamilies(), text_style.getFontStyle());
}

void CanvasRenderingContext2D::render_to_canvas(SkPaint& paint, std::function<void (SkPaint&)> f) {
    CanvasState state = this->states_.top();

    std::shared_ptr<SkPaint> shadow_paint = node_skia::StyleParser::getShadowLayerPaint(
        paint,
        state.shadow_color,
        state.shadow_blur,
        state.shadow_offset_x,
        state.shadow_offset_y
    );

    if (shadow_paint != nullptr) {
        this->canvas_->save();
        this->canvas_->translate(state.shadow_offset_x, state.shadow_offset_y);

        f(*shadow_paint);

        this->canvas_->restore();
    }

    f(paint);
}

void CanvasRenderingContext2D::fill_with_dye(SkPaint& paint, Napi::Value value) {
    if (value.IsObject()) {
        Napi::Object dye = value.As<Napi::Object>();
        
        if (dye.InstanceOf(CanvasGradient::constructor.Value())) {
            // maybe gradient
            CanvasGradient* gradient = Napi::ObjectWrap<CanvasGradient>::Unwrap(dye);
            int pos_count = gradient->colors.size();

            if (pos_count >= 2) {
                sk_sp<SkShader> shader = nullptr;

                if (gradient->gradient_type == GradientType::Linear) {
                    shader = SkGradientShader::MakeLinear(
                        gradient->pts,
                        gradient->getSortedGradientColors().data(),
                        gradient->getSortedGradientPos().data(),
                        gradient->colors.size(),
                        SkTileMode::kClamp
                    );
                } else if (gradient->gradient_type == GradientType::Radial) {
                    shader = SkGradientShader::MakeTwoPointConical(
                        gradient->pts[0],
                        gradient->radius[0],
                        gradient->pts[1],
                        gradient->radius[1],
                        gradient->getSortedGradientColors().data(),
                        gradient->getSortedGradientPos().data(),
                        gradient->colors.size(),
                        SkTileMode::kClamp
                    );
                }
                
                paint.setShader(shader);
                // set a non-transparent color here
                paint.setColor4f(SkColors::kBlack);
            } else if (pos_count == 1) {
                paint.setShader(nullptr);
                paint.setColor4f(gradient->colors[0].color);
            }
        } else if (dye.InstanceOf(CanvasPattern::constructor.Value())) {
            CanvasPattern* pattern = Napi::ObjectWrap<CanvasPattern>::Unwrap(dye);
            paint.setShader(pattern->toShader(this->states_.top().image_smoothing_enable));
        }
    } else if (value.IsString()) {
        // color
        string fill_style = value.ToString().Utf8Value();
        shared_ptr<SkColor4f> fill_style_color = W3CSkColorParser::rgba_from_string(fill_style);

        if (fill_style_color != nullptr) {
            W3CSkColorParser::color_mix_with_alpha(*fill_style_color, this->states_.top().global_alpha_);

            paint.setShader(nullptr);
            paint.setColor4f(*fill_style_color);
        }
    }
}
