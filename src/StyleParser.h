#pragma once

#include <string>
#include "include/core/SkPaint.h"
#include "include/core/SkFontMetrics.h"
#include "modules/skparagraph/include/DartTypes.h"

using std::string;
using skia::textlayout::TextAlign;

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
    static std::shared_ptr<SkPaint> getShadowLayerPaint(SkPaint& base_paint, SkColor4f color, double blur, double x, double y);

    static float getBaselineOffsetFromFontMetrics(SkFontMetrics& font_metrics, TextBaseline& baseline);
};

}
