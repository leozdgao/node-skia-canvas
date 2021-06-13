#include <iostream>

#include <include/core/SkTextBlob.h>

#include "CanvasRenderingContext2D.h"
#include "StyleParser.h"
#include "W3CSkColorParser.h"
#include "helpers.h"

using node_skia::StyleParser;

CanvasRenderingContext2D::CanvasRenderingContext2D() {
    // 初始化 ctx 内部绘制状态
    paint_for_fill_ = SkPaint();
    paint_for_fill_.setStyle(SkPaint::kFill_Style);

    paint_for_stroke_ = SkPaint();
    paint_for_stroke_.setStyle(SkPaint::kStroke_Style);

    pargf_style_ = ParagraphStyle();
    text_style_ = TextStyle();
    text_style_.setFontSize(10);
    text_style_.setFontFamilies({ SkString("sans-serif") });
    text_baseline_ = TextBaseline::Alphabetic;
}

CanvasRenderingContext2D::~CanvasRenderingContext2D() {
    napi_delete_reference(env_, wrapper_);
}

napi_status CanvasRenderingContext2D::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor properties[] = {
        // properties
        DECLARE_NAPI_PROPERTY("fillStyle", GetFillStyle, SetFillStyle),
        DECLARE_NAPI_PROPERTY("lineWidth", GetLineWidth, SetLineWidth),
        DECLARE_NAPI_PROPERTY("strokeStyle", GetStrokeStyle, SetStrokeStyle),
        DECLARE_NAPI_PROPERTY("textAlign", GetTextAlign, SetTextAlign),
        DECLARE_NAPI_PROPERTY("textBaseline", GetTextBaseline, SetTextBaseline),
        // methods
        DECLARE_NAPI_METHOD("clearRect", ClearRect),
        DECLARE_NAPI_METHOD("fillRect", FillRect),
        DECLARE_NAPI_METHOD("fillWithPath2D", FillWithPath2D),
        DECLARE_NAPI_METHOD("fillText", FillText),
        DECLARE_NAPI_METHOD("strokeRect", StrokeRect),
        DECLARE_NAPI_METHOD("strokeWithPath2D", StrokeWithPath2D),
        DECLARE_NAPI_METHOD("strokeText", StrokeText),
        DECLARE_NAPI_METHOD("measureText", MeasureText),
    };

    napi_value cons;
    status = napi_define_class(env, "CanvasRenderingContext2D", NAPI_AUTO_LENGTH, New, nullptr, 13, properties, &cons);
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

void CanvasRenderingContext2D::SetCanvas(SkCanvas* canvas) {
    canvas_ = canvas;
};

// ================================== Properties ==================================

napi_value CanvasRenderingContext2D::GetFillStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->paint_for_fill_.getColor4f();

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

    string fill_style = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkColor4f fill_style_color = W3CSkColorParser::rgba_from_string(fill_style);

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->paint_for_fill_.setColor4f(fill_style_color);
}

napi_value CanvasRenderingContext2D::GetLineWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->paint_for_stroke_.getStrokeWidth();
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
    
    ctx->paint_for_stroke_.setStrokeWidth(line_width);
}

napi_value CanvasRenderingContext2D::GetStrokeStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->paint_for_stroke_.getColor4f();

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

    string stroke_style = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkColor4f stroke_style_color = W3CSkColorParser::rgba_from_string(stroke_style);

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->paint_for_stroke_.setColor4f(stroke_style_color);
}

napi_value CanvasRenderingContext2D::GetTextAlign(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextAlign align = ctx->pargf_style_.getTextAlign();
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
    
    ctx->pargf_style_.setTextAlign(StyleParser::fromStrToTextAlign(text_align));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextBaseline(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextBaseline baseline = ctx->text_baseline_;
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
    
    ctx->text_baseline_ = StyleParser::fromStrToTextBaseline(text_baseline);

    return nullptr;
}

// ================================== Methods ==================================

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

    ctx->canvas_->drawRect(rect, ctx->paint_for_fill_);
}

napi_value CanvasRenderingContext2D::FillWithPath2D(napi_env env, napi_callback_info info) {

}

napi_value CanvasRenderingContext2D::FillText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string input = node_skia_helpers::get_utf8_string(env, argv[0]);
    auto text = SkTextBlob::MakeFromString(input.data(), SkFont(nullptr, 18));

    ctx->canvas_->drawTextBlob(text.get(), 50, 25, ctx->paint_for_fill_);
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

    ctx->canvas_->drawRect(rect, ctx->paint_for_stroke_);
}

napi_value CanvasRenderingContext2D::StrokeWithPath2D(napi_env env, napi_callback_info info) {

}

napi_value CanvasRenderingContext2D::StrokeText(napi_env env, napi_callback_info info) {

}

napi_value CanvasRenderingContext2D::MeasureText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string text = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkFontMetrics font_metrics;
    ctx->text_style_.getFontMetrics(&font_metrics);
    // 新基线到原基线的距离
    float offset = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, ctx->text_baseline_);
    auto base_hang = TextBaseline::Hanging;
    float hang = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, base_hang) - offset;
    auto base_ideo = TextBaseline::Ideographic;
    float ideo = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, base_ideo) - offset;
    // 原基线需要偏移的距离
    float norm = 0 - offset;
    float ascent = norm - font_metrics.fAscent;
    float descent = font_metrics.fDescent - norm;
    

    // FIXME: font
    // sk_sp<SkTextBlob> tb = SkTextBlob::MakeFromString(text.data(), SkFont(nullptr, 10));
    // SkRect bounds = tb->bounds();

    napi_value text_metrics, v_norm, v_ascent, v_descent, v_hang, v_ideo;
    status = napi_create_double(env, norm, &v_norm);
    status = napi_create_double(env, ascent, &v_ascent);
    status = napi_create_double(env, descent, &v_descent);
    status = napi_create_double(env, hang, &v_hang);
    status = napi_create_double(env, ideo, &v_ideo);

    status = napi_create_object(env, &text_metrics);
    status = napi_set_named_property(env, text_metrics, "actualBoundingBoxAscent", v_ascent);
    status = napi_set_named_property(env, text_metrics, "actualBoundingBoxDescent", v_descent);
    status = napi_set_named_property(env, text_metrics, "emHeightAscent", v_ascent);
    status = napi_set_named_property(env, text_metrics, "emHeightDescent", v_descent);
    status = napi_set_named_property(env, text_metrics, "hangingBaseline", v_hang);
    status = napi_set_named_property(env, text_metrics, "alphabeticBaseline", v_norm);
    status = napi_set_named_property(env, text_metrics, "ideographicBaseline", v_ideo);

    return text_metrics;
}
