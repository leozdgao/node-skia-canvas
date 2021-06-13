#pragma once

#include <string>
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

    static TextAlign fromStrToTextAlign(string& align);
    static string fromTextAlignToStr(TextAlign& align);
    static TextBaseline fromStrToTextBaseline(string& baseline);
    static string fromTextBaselineToStr(TextBaseline& baseline);

    static float getBaselineOffsetFromFontMetrics(SkFontMetrics& font_metrics, TextBaseline& baseline);
};

}
