#include "include/effects/SkImageFilters.h"
#include "StyleParser.h"

using skia::textlayout::TextDirection;

namespace node_skia {

SkPaint::Cap StyleParser::fromStrToStrokeCap(string& cap) {
    if (cap == "butt") {
        return SkPaint::Cap::kButt_Cap;
    }

    if (cap == "round") {
        return SkPaint::Cap::kRound_Cap;
    }

    if (cap == "square") {
        return SkPaint::Cap::kSquare_Cap;
    }

    return SkPaint::Cap::kDefault_Cap;
}

string StyleParser::fromStrokeCapToStr(SkPaint::Cap& cap) {
    if (cap == SkPaint::Cap::kButt_Cap) {
        return "butt";
    }

    if (cap == SkPaint::Cap::kRound_Cap) {
        return "round";
    }

    if (cap == SkPaint::Cap::kSquare_Cap) {
        return "square";
    }

    return "butt";
}

SkPaint::Join StyleParser::fromStrToStrokeJoin(string& join) {
    if (join == "bevel") {
        return SkPaint::Join::kBevel_Join;
    }

    if (join == "round") {
        return SkPaint::Join::kRound_Join;
    }

    if (join == "miter") {
        return SkPaint::Join::kMiter_Join;
    }

    return SkPaint::kDefault_Join;
}

string StyleParser::fromStrokeJoinToStr(SkPaint::Join& join) {
    if (join == SkPaint::Join::kBevel_Join) {
        return "bevel";
    }

    if (join == SkPaint::Join::kRound_Join) {
        return "round";
    }

    if (join == SkPaint::Join::kMiter_Join) {
        return "miter";
    }

    return "miter";
}

TextAlign StyleParser::fromStrToTextAlign(string& align_name, TextAlign default_value) {
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

    return default_value;
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

std::shared_ptr<SkPaint> StyleParser::getShadowLayerPaint(SkPaint& base_paint, SkColor4f color, double blur, double x, double y) {
    if (color.fA > 0 && !(blur == 0.0 && x == 0.0 && y == 0.0)) {
        std::shared_ptr<SkPaint> layer_paint(new SkPaint());
        // copy
        *layer_paint = base_paint;

        double sigma = blur / 2.0;
        sk_sp<SkImageFilter> shadow_filter = SkImageFilters::DropShadowOnly(0, 0, sigma, sigma, color.toSkColor(), nullptr, nullptr);
        layer_paint->setImageFilter(shadow_filter);

        return layer_paint;
    }

    return nullptr;
}

void StyleParser::fillTileModeWithRepetition(string& repetition, SkTileMode* result) {
    if (repetition == "repeat-x") {
        result[0] = SkTileMode::kRepeat;
        result[1] = SkTileMode::kDecal;
    } else if (repetition == "repeat-y") {
        result[0] = SkTileMode::kDecal;
        result[1] = SkTileMode::kRepeat;
    } else if (repetition == "no-repeat") {
        result[0] = SkTileMode::kDecal;
        result[1] = SkTileMode::kDecal;
    } else {
        result[0] = SkTileMode::kRepeat;
        result[1] = SkTileMode::kRepeat;
    }
}

SkFontStyle::Slant StyleParser::fromStrToFontSlant(string& slant) {
    if (slant == "normal") {
        return SkFontStyle::Slant::kUpright_Slant;
    }

    if (slant == "italic") {
        return SkFontStyle::Slant::kItalic_Slant;
    }

    if (slant == "oblique") {
        return SkFontStyle::Slant::kOblique_Slant;
    }
}

SkFontStyle::Width StyleParser::fromStrToFontStrecth(string& stretch) {
    if (stretch == "ultra-condensed") return SkFontStyle::Width::kUltraCondensed_Width;
    if (stretch == "extra-condensed") return SkFontStyle::Width::kExtraCondensed_Width;
    if (stretch == "condensed") return SkFontStyle::Width::kCondensed_Width;
    if (stretch == "semi-condensed") return SkFontStyle::Width::kSemiCondensed_Width;
    if (stretch == "semi-expanded") return SkFontStyle::Width::kSemiExpanded_Width;
    if (stretch == "expanded") return SkFontStyle::Width::kExpanded_Width;
    if (stretch == "extra-expanded") return SkFontStyle::Width::kExtraExpanded_Width;
    if (stretch == "ultra-expanded") return SkFontStyle::Width::kUltraExpanded_Width;

    return SkFontStyle::Width::kNormal_Width;
}

SkScalar StyleParser::getTextBaselineOffset(SkFontMetrics& metrics, TextBaseline baseline) {
    if (baseline == TextBaseline::Top) return 0;
    if (baseline == TextBaseline::Hanging) return -(-metrics.fAscent - metrics.fCapHeight);
    if (baseline == TextBaseline::Middle) return -(-metrics.fAscent - metrics.fCapHeight / 2);
    if (baseline == TextBaseline::Alphabetic) return metrics.fAscent;
    if (baseline == TextBaseline::Ideographic) return -(-metrics.fAscent + metrics.fDescent);
    if (baseline == TextBaseline::Bottom) return -(-metrics.fAscent + metrics.fDescent);
    
    return 0;
}

SkScalar StyleParser::getTextAlignFactor(ParagraphStyle& style) {
    TextDirection dir = style.getTextDirection();
    TextAlign align = style.getTextAlign();

    if (dir == TextDirection::kLtr) {
        if (align == TextAlign::kLeft || align == TextAlign::kStart) return 0;
        if (align == TextAlign::kRight || align == TextAlign::kEnd) return -1;
        if (align == TextAlign::kCenter) return -0.5;
    }

    if (dir == TextDirection::kRtl) {
        if (align == TextAlign::kLeft || align == TextAlign::kEnd) return 0;
        if (align == TextAlign::kRight || align == TextAlign::kStart) return -1;
        if (align == TextAlign::kCenter) return -0.5;
    }

    return 0;
}

}
