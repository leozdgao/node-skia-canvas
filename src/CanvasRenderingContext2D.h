#pragma once

#include <stack>
#include <string>
#include <vector>
#include <napi.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPath.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "StyleParser.h"

using std::stack;
using std::string;
using std::vector;

using skia::textlayout::ParagraphStyle;
using skia::textlayout::TextStyle;
using node_skia::TextBaseline;


struct CanvasState {
    // state for path
    SkPath path_;
    SkPoint* last_move_point_ = nullptr;
    
    // state for dash
    vector<SkScalar> intervals = {};
    double line_dash_offset = 0.0;

    // state for fill & stroke
    SkPaint paint_for_fill_;
    SkPaint paint_for_stroke_;

    // state for alpha
    double global_alpha_ = 1.0;

    // state for shadow
    double shadow_blur = 0.0;
    SkColor4f shadow_color = SkColors::kTransparent;
    double shadow_offset_x = 0.0;
    double shadow_offset_y = 0.0;

    bool image_smoothing_enable = false;

    // state for text
    string font_str = "10px sans-serif";
    ParagraphStyle pargf_style_;
    TextStyle text_style_;
    TextBaseline text_baseline_;
    sk_sp<SkTypeface> typeface_;

    // transform
    SkMatrix matrix_ = SkMatrix();
};


class CanvasRenderingContext2D {
public:
    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    void setCanvas(SkCanvas* canvas);
    void resetStates();

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    napi_env env_;
    napi_ref wrapper_;

    SkCanvas* canvas_;

    stack<CanvasState> states_;

    void init_canvas_state();

    void fill_with_dye(SkPaint& paint, Napi::Value dye);
    void render_text(SkPaint& paint, string text, SkScalar x, SkScalar y, SkScalar maxWidth);
    void render_to_canvas(SkPaint& paint, std::function<void (SkPaint&)> f);
    vector<double> measure_text(SkPaint& paint, string text, SkScalar maxWidth);

    // ================================== Properties ==================================

    static napi_value GetFillStyle(napi_env env, napi_callback_info info);
    static napi_value SetFillStyle(napi_env env, napi_callback_info info);
    static napi_value GetFont(napi_env env, napi_callback_info info);
    static napi_value SetFont(napi_env env, napi_callback_info info);
    static napi_value GetGlobalAlpha(napi_env env, napi_callback_info info);
    static napi_value SetGlobalAlpha(napi_env env, napi_callback_info info);
    static napi_value GetImageSmoothingEnabled(napi_env env, napi_callback_info info);
    static napi_value SetImageSmoothingEnabled(napi_env env, napi_callback_info info);
    static napi_value GetLineCap(napi_env env, napi_callback_info info);
    static napi_value SetLineCap(napi_env env, napi_callback_info info);
    static napi_value GetLineDashOffset(napi_env env, napi_callback_info info);
    static napi_value SetLineDashOffset(napi_env env, napi_callback_info info);
    static napi_value GetLineJoin(napi_env env, napi_callback_info info);
    static napi_value SetLineJoin(napi_env env, napi_callback_info info);
    static napi_value GetLineWidth(napi_env env, napi_callback_info info);
    static napi_value SetLineWidth(napi_env env, napi_callback_info info);
    static napi_value GetMiterLimit(napi_env env, napi_callback_info info);
    static napi_value SetMiterLimit(napi_env env, napi_callback_info info);
    static napi_value GetShadowBlur(napi_env env, napi_callback_info info);
    static napi_value SetShadowBlur(napi_env env, napi_callback_info info);
    static napi_value GetShadowColor(napi_env env, napi_callback_info info);
    static napi_value SetShadowColor(napi_env env, napi_callback_info info);
    static napi_value GetShadowOffsetX(napi_env env, napi_callback_info info);
    static napi_value SetShadowOffsetX(napi_env env, napi_callback_info info);
    static napi_value GetShadowOffsetY(napi_env env, napi_callback_info info);
    static napi_value SetShadowOffsetY(napi_env env, napi_callback_info info);
    static napi_value GetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value SetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value GetTextAlign(napi_env env, napi_callback_info info);
    static napi_value SetTextAlign(napi_env env, napi_callback_info info);
    static napi_value GetTextBaseline(napi_env env, napi_callback_info info);
    static napi_value SetTextBaseline(napi_env env, napi_callback_info info);
    static napi_value GetTextDecoration(napi_env env, napi_callback_info info);
    static napi_value SetTextDecoration(napi_env env, napi_callback_info info);

    // ================================== Methods ==================================

    static napi_value Arc(napi_env env, napi_callback_info info);
    static napi_value ArcTo(napi_env env, napi_callback_info info);
    static napi_value BeginPath(napi_env env, napi_callback_info info);
    static napi_value BezierCurveTo(napi_env env, napi_callback_info info);
    static napi_value CreateImageData(napi_env env, napi_callback_info info);
    static napi_value ClearRect(napi_env env, napi_callback_info info);
    static napi_value Clip(napi_env env, napi_callback_info info);
    static napi_value ClosePath(napi_env env, napi_callback_info info);
    static napi_value DrawImage(napi_env env, napi_callback_info info);
    static napi_value Ellipse(napi_env env, napi_callback_info info);
    static napi_value Fill(napi_env env, napi_callback_info info);
    static napi_value FillRect(napi_env env, napi_callback_info info);
    static napi_value FillText(napi_env env, napi_callback_info info);
    static napi_value GetImageData(napi_env env, napi_callback_info info);
    static napi_value GetLineDash(napi_env env, napi_callback_info info);
    static napi_value GetTransform(napi_env env, napi_callback_info info);
    static napi_value LineTo(napi_env env, napi_callback_info info);
    static napi_value MoveTo(napi_env env, napi_callback_info info);
    static napi_value PutImageData(napi_env env, napi_callback_info info);
    static napi_value QuadraticCurveTo(napi_env env, napi_callback_info info);
    static napi_value Rect(napi_env env, napi_callback_info info);
    static napi_value ResetTransform(napi_env env, napi_callback_info info);
    static napi_value Restore(napi_env env, napi_callback_info info);
    static napi_value Rotate(napi_env env, napi_callback_info info);
    static napi_value Save(napi_env env, napi_callback_info info);
    static napi_value Scale(napi_env env, napi_callback_info info);
    static napi_value SetLineDash(napi_env env, napi_callback_info info);
    static napi_value SetTransform(napi_env env, napi_callback_info info);
    static napi_value Stroke(napi_env env, napi_callback_info info);
    static napi_value StrokeRect(napi_env env, napi_callback_info info);
    static napi_value StrokeText(napi_env env, napi_callback_info info);
    static napi_value Transform(napi_env env, napi_callback_info info);
    static napi_value Translate(napi_env env, napi_callback_info info);

    static napi_value MeasureText(napi_env env, napi_callback_info info);
};

