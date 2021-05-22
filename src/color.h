#ifndef NODE_SKIA_COLOR_H
#define NODE_SKIA_COLOR_H

#include <string>
#include <unordered_map>
#include <include/core/SkColor.h>

using std::string;
using std::unordered_map;
using SkColorMap = unordered_map<string, SkColor>;

namespace node_skia_color {
    SkColorMap semantic_color_map = {
        { "transparent", SK_ColorTRANSPARENT },
        { "black", SK_ColorBLACK },
        { "dkgray", SK_ColorDKGRAY },
        { "gray", SK_ColorGRAY },
        { "ltgray", SK_ColorLTGRAY },
        { "white", SK_ColorWHITE },
        { "red", SK_ColorRED },
        { "green", SK_ColorGREEN },
        { "blue", SK_ColorBLUE },
        { "yellow", SK_ColorYELLOW },
        { "cyan", SK_ColorCYAN },
        { "magenta", SK_ColorMAGENTA }
    };
}

#endif
