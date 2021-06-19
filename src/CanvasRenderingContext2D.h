#pragma once

#include <napi.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "StyleParser.h"

using skia::textlayout::ParagraphStyle;
using skia::textlayout::TextStyle;

using node_skia::TextBaseline;

class CanvasRenderingContext2D {
public:
    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    void SetCanvas(SkCanvas* canvas);

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    napi_env env_;
    napi_ref wrapper_;

    SkCanvas* canvas_;

    // state for fill & stroke
    SkPaint paint_for_fill_;
    SkPaint paint_for_stroke_;

    // state for text
    ParagraphStyle pargf_style_;
    TextStyle text_style_;
    TextBaseline text_baseline_;

    // ================================== Properties ==================================

    static napi_value GetFillStyle(napi_env env, napi_callback_info info);
    static napi_value SetFillStyle(napi_env env, napi_callback_info info);
    static napi_value GetLineWidth(napi_env env, napi_callback_info info);
    static napi_value SetLineWidth(napi_env env, napi_callback_info info);
    static napi_value GetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value SetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value GetTextAlign(napi_env env, napi_callback_info info);
    static napi_value SetTextAlign(napi_env env, napi_callback_info info);
    static napi_value GetTextBaseline(napi_env env, napi_callback_info info);
    static napi_value SetTextBaseline(napi_env env, napi_callback_info info);

    // ================================== Methods ==================================

    static napi_value ClearRect(napi_env env, napi_callback_info info);
    static napi_value DrawImage(napi_env env, napi_callback_info info);
    static napi_value FillRect(napi_env env, napi_callback_info info);
    static napi_value FillWithPath2D(napi_env env, napi_callback_info info); // work for `ctx.fill()`
    static napi_value FillText(napi_env env, napi_callback_info info);
    static napi_value StrokeRect(napi_env env, napi_callback_info info);
    static napi_value StrokeWithPath2D(napi_env env, napi_callback_info info); // work for `ctx.stroke()`
    static napi_value StrokeText(napi_env env, napi_callback_info info);

    static napi_value MeasureText(napi_env env, napi_callback_info info);
};

