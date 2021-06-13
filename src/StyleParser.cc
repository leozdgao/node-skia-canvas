#include "StyleParser.h"

namespace node_skia {

TextAlign StyleParser::fromStrToTextAlign(string& align_name) {
    if (align_name == "center") {
        return TextAlign::kCenter;
    }

    if (align_name == "end") {
        return TextAlign::kEnd;
    }

    if (align_name == "justify") {
        return TextAlign::kJustify;
    }

    if (align_name == "left") {
        return TextAlign::kLeft;
    }

    if (align_name == "right") {
        return TextAlign::kRight;
    }

    if (align_name == "start") {
        return TextAlign::kStart;
    }

    return TextAlign::kStart;
}

string StyleParser::fromTextAlignToStr(TextAlign& align) {
    if (align == TextAlign::kCenter) {
        return "center";
    }

    if (align == TextAlign::kEnd) {
        return "end";
    }

    if (align == TextAlign::kJustify) {
        return "justify";
    }

    if (align == TextAlign::kLeft) {
        return "left";
    }

    if (align == TextAlign::kRight) {
        return "right";
    }

    if (align == TextAlign::kStart) {
        return "start";
    }

    return "start";
}

TextBaseline StyleParser::fromStrToTextBaseline(string& baseline) {
    if (baseline == "top") {
        return TextBaseline::Top;
    }

    if (baseline == "hanging") {
        return TextBaseline::Hanging;
    }

    if (baseline == "middle") {
        return TextBaseline::Middle;
    }

    if (baseline == "alphabetic") {
        return TextBaseline::Alphabetic;
    }

    if (baseline == "ideographic") {
        return TextBaseline::Ideographic;
    }

    if (baseline == "bottom") {
        return TextBaseline::Bottom;
    }

    return TextBaseline::Alphabetic;
}

string StyleParser::fromTextBaselineToStr(TextBaseline& baseline) {
    if (baseline == TextBaseline::Top) {
        return "top";
    }

    if (baseline == TextBaseline::Hanging) {
        return "hanging";
    }

    if (baseline == TextBaseline::Middle) {
        return "middle";
    }

    if (baseline == TextBaseline::Alphabetic) {
        return "alphabetic";
    }

    if (baseline == TextBaseline::Ideographic) {
        return "ideographic";
    }

    if (baseline == TextBaseline::Bottom) {
        return "bottom";
    }

    return "alphabetic";
}

float StyleParser::getBaselineOffsetFromFontMetrics(SkFontMetrics& font_metrics, TextBaseline& baseline) {
    if (baseline == TextBaseline::Alphabetic) {
        return 0;
    }

    if (baseline == TextBaseline::Top) {
        return -font_metrics.fAscent;
    }

    if (baseline == TextBaseline::Hanging) {
        return font_metrics.fCapHeight;
    }

    if (baseline == TextBaseline::Middle) {
        return font_metrics.fCapHeight / 2;
    }

    if (baseline == TextBaseline::Ideographic) {
        return -font_metrics.fDescent;
    }

    if (baseline == TextBaseline::Bottom) {
        return -font_metrics.fDescent;
    }
}

}
