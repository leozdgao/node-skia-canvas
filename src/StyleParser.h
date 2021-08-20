#pragma once

#include <string>
#include "include/core/SkPaint.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkFontStyle.h"
#include "modules/skparagraph/include/DartTypes.h"
#include "modules/skparagraph/include/ParagraphStyle.h"

using std::string;
using skia::textlayout::TextAlign;
using skia::textlayout::ParagraphStyle;

namespace node_skia {

enum class TextBaseline {
    Top,
    Hanging,
    Middle,
    Alphabetic,
    Ideographic,
    Bottom,
};

class StyleParser {
private:
    /* data */
public:
    StyleParser(/* args */);
    ~StyleParser();

    static SkPaint::Cap fromStrToStrokeCap(string& cap);
    static string fromStrokeCapToStr(SkPaint::Cap& cap);
    static SkPaint::Join fromStrToStrokeJoin(string& join);
    static string fromStrokeJoinToStr(SkPaint::Join& join);
    static TextAlign fromStrToTextAlign(string& align);
    static string fromTextAlignToStr(TextAlign& align);
    static TextBaseline fromStrToTextBaseline(string& baseline);
    static string fromTextBaselineToStr(TextBaseline& baseline);
    static void fillTileModeWithRepetition(string& repetition, SkTileMode* result);
    static SkFontStyle::Slant fromStrToFontSlant(string& slant);
    static SkFontStyle::Width fromStrToFontStrecth(string& stretch);

    static SkScalar getTextBaselineOffset(SkFontMetrics& metrics, TextBaseline baseline);
    static SkScalar getTextAlignFactor(ParagraphStyle& style);
    static std::shared_ptr<SkPaint> getShadowLayerPaint(SkPaint& base_paint, SkColor4f color, double blur, double x, double y);
};

}
