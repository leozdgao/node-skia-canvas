#include <algorithm>
#include <iostream>

#include "W3CSkColorParser.h"
#include "helpers.h"

using std::round;
using std::stoi;
using std::stof;

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it) || *it == '.' || *it == '-')) ++it;
    return !s.empty() && it == s.end();
}

const SkColorMap W3CSkColorParser::semantic_color_map = {
    // 'transparent' keyword
    { "transparent", SK_ColorTRANSPARENT },
    /**
     * @desc: basic color keywords * extended color keywords
     * @doc: https://www.w3.org/TR/2018/REC-css-color-3-20180619/#svg-color
     */
    { "aliceblue", SkColorSetARGB(0xFF, 0xF0, 0xF8, 0xFF) },
    { "antiquewhite", SkColorSetARGB(0xFF, 0xFA, 0xEB, 0xD7) },
    { "aqua", SkColorSetARGB(0xFF, 0x00, 0xFF, 0xFF) },
    { "aquamarine", SkColorSetARGB(0xFF, 0x7F, 0xFF, 0xD4) },
    { "azure", SkColorSetARGB(0xFF, 0xF0, 0xFF, 0xFF) },
    { "beige", SkColorSetARGB(0xFF, 0xF5, 0xF5, 0xDC) },
    { "bisque", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xC4) },
    { "black", SkColorSetARGB(0xFF, 0x00, 0x00, 0x00) },
    { "blanchedalmond", SkColorSetARGB(0xFF, 0xFF, 0xEB, 0xCD) },
    { "blue", SkColorSetARGB(0xFF, 0x00, 0x00, 0xFF) },
    { "blueviolet", SkColorSetARGB(0xFF, 0x8A, 0x2B, 0xE2) },
    { "brown", SkColorSetARGB(0xFF, 0xA5, 0x2A, 0x2A) },
    { "burlywood", SkColorSetARGB(0xFF, 0xDE, 0xB8, 0x87) },
    { "cadetblue", SkColorSetARGB(0xFF, 0x5F, 0x9E, 0xA0) },
    { "chartreuse", SkColorSetARGB(0xFF, 0x7F, 0xFF, 0x00) },
    { "chocolate", SkColorSetARGB(0xFF, 0xD2, 0x69, 0x1E) },
    { "coral", SkColorSetARGB(0xFF, 0xFF, 0x7F, 0x50) },
    { "cornflowerblue", SkColorSetARGB(0xFF, 0x64, 0x95, 0xED) },
    { "cornsilk", SkColorSetARGB(0xFF, 0xFF, 0xF8, 0xDC) },
    { "crimson", SkColorSetARGB(0xFF, 0xDC, 0x14, 0x3C) },
    { "cyan", SkColorSetARGB(0xFF, 0x00, 0xFF, 0xFF) },
    { "darkblue", SkColorSetARGB(0xFF, 0x00, 0x00, 0x8B) },
    { "darkcyan", SkColorSetARGB(0xFF, 0x00, 0x8B, 0x8B) },
    { "darkgoldenrod", SkColorSetARGB(0xFF, 0xB8, 0x86, 0x0B) },
    { "darkgray", SkColorSetARGB(0xFF, 0xA9, 0xA9, 0xA9) },
    { "darkgreen", SkColorSetARGB(0xFF, 0x00, 0x64, 0x00) },
    { "darkgrey", SkColorSetARGB(0xFF, 0xA9, 0xA9, 0xA9) },
    { "darkkhaki", SkColorSetARGB(0xFF, 0xBD, 0xB7, 0x6B) },
    { "darkmagenta", SkColorSetARGB(0xFF, 0x8B, 0x00, 0x8B) },
    { "darkolivegreen", SkColorSetARGB(0xFF, 0x55, 0x6B, 0x2F) },
    { "darkorange", SkColorSetARGB(0xFF, 0xFF, 0x8C, 0x00) },
    { "darkorchid", SkColorSetARGB(0xFF, 0x99, 0x32, 0xCC) },
    { "darkred", SkColorSetARGB(0xFF, 0x8B, 0x00, 0x00) },
    { "darksalmon", SkColorSetARGB(0xFF, 0xE9, 0x96, 0x7A) },
    { "darkseagreen", SkColorSetARGB(0xFF, 0x8F, 0xBC, 0x8F) },
    { "darkslateblue", SkColorSetARGB(0xFF, 0x48, 0x3D, 0x8B) },
    { "darkslategray", SkColorSetARGB(0xFF, 0x2F, 0x4F, 0x4F) },
    { "darkslategrey", SkColorSetARGB(0xFF, 0x2F, 0x4F, 0x4F) },
    { "darkturquoise", SkColorSetARGB(0xFF, 0x00, 0xCE, 0xD1) },
    { "darkviolet", SkColorSetARGB(0xFF, 0x94, 0x00, 0xD3) },
    { "deeppink", SkColorSetARGB(0xFF, 0xFF, 0x14, 0x93) },
    { "deepskyblue", SkColorSetARGB(0xFF, 0x00, 0xBF, 0xFF) },
    { "dimgray", SkColorSetARGB(0xFF, 0x69, 0x69, 0x69) },
    { "dimgrey", SkColorSetARGB(0xFF, 0x69, 0x69, 0x69) },
    { "dodgerblue", SkColorSetARGB(0xFF, 0x1E, 0x90, 0xFF) },
    { "firebrick", SkColorSetARGB(0xFF, 0xB2, 0x22, 0x22) },
    { "floralwhite", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xF0) },
    { "forestgreen", SkColorSetARGB(0xFF, 0x22, 0x8B, 0x22) },
    { "fuchsia", SkColorSetARGB(0xFF, 0xFF, 0x00, 0xFF) },
    { "gainsboro", SkColorSetARGB(0xFF, 0xDC, 0xDC, 0xDC) },
    { "ghostwhite", SkColorSetARGB(0xFF, 0xF8, 0xF8, 0xFF) },
    { "gold", SkColorSetARGB(0xFF, 0xFF, 0xD7, 0x00) },
    { "goldenrod", SkColorSetARGB(0xFF, 0xDA, 0xA5, 0x20) },
    { "gray", SkColorSetARGB(0xFF, 0x80, 0x80, 0x80) },
    { "green", SkColorSetARGB(0xFF, 0x00, 0x80, 0x00) },
    { "greenyellow", SkColorSetARGB(0xFF, 0xAD, 0xFF, 0x2F) },
    { "grey", SkColorSetARGB(0xFF, 0x80, 0x80, 0x80) },
    { "honeydew", SkColorSetARGB(0xFF, 0xF0, 0xFF, 0xF0) },
    { "hotpink", SkColorSetARGB(0xFF, 0xFF, 0x69, 0xB4) },
    { "indianred", SkColorSetARGB(0xFF, 0xCD, 0x5C, 0x5C) },
    { "indigo", SkColorSetARGB(0xFF, 0x4B, 0x00, 0x82) },
    { "ivory", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xF0) },
    { "khaki", SkColorSetARGB(0xFF, 0xF0, 0xE6, 0x8C) },
    { "lavender", SkColorSetARGB(0xFF, 0xE6, 0xE6, 0xFA) },
    { "lavenderblush", SkColorSetARGB(0xFF, 0xFF, 0xF0, 0xF5) },
    { "lawngreen", SkColorSetARGB(0xFF, 0x7C, 0xFC, 0x00) },
    { "lemonchiffon", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xCD) },
    { "lightblue", SkColorSetARGB(0xFF, 0xAD, 0xD8, 0xE6) },
    { "lightcoral", SkColorSetARGB(0xFF, 0xF0, 0x80, 0x80) },
    { "lightcyan", SkColorSetARGB(0xFF, 0xE0, 0xFF, 0xFF) },
    { "lightgoldenrodyellow", SkColorSetARGB(0xFF, 0xFA, 0xFA, 0xD2) },
    { "lightgray", SkColorSetARGB(0xFF, 0xD3, 0xD3, 0xD3) },
    { "lightgreen", SkColorSetARGB(0xFF, 0x90, 0xEE, 0x90) },
    { "lightgrey", SkColorSetARGB(0xFF, 0xD3, 0xD3, 0xD3) },
    { "lightpink", SkColorSetARGB(0xFF, 0xFF, 0xB6, 0xC1) },
    { "lightsalmon", SkColorSetARGB(0xFF, 0xFF, 0xA0, 0x7A) },
    { "lightseagreen", SkColorSetARGB(0xFF, 0x20, 0xB2, 0xAA) },
    { "lightskyblue", SkColorSetARGB(0xFF, 0x87, 0xCE, 0xFA) },
    { "lightslategray", SkColorSetARGB(0xFF, 0x77, 0x88, 0x99) },
    { "lightslategrey", SkColorSetARGB(0xFF, 0x77, 0x88, 0x99) },
    { "lightsteelblue", SkColorSetARGB(0xFF, 0xB0, 0xC4, 0xDE) },
    { "lightyellow", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xE0) },
    { "lime", SkColorSetARGB(0xFF, 0x00, 0xFF, 0x00) },
    { "limegreen", SkColorSetARGB(0xFF, 0x32, 0xCD, 0x32) },
    { "linen", SkColorSetARGB(0xFF, 0xFA, 0xF0, 0xE6) },
    { "magenta", SkColorSetARGB(0xFF, 0xFF, 0x00, 0xFF) },
    { "maroon", SkColorSetARGB(0xFF, 0x80, 0x00, 0x00) },
    { "mediumaquamarine", SkColorSetARGB(0xFF, 0x66, 0xCD, 0xAA) },
    { "mediumblue", SkColorSetARGB(0xFF, 0x00, 0x00, 0xCD) },
    { "mediumorchid", SkColorSetARGB(0xFF, 0xBA, 0x55, 0xD3) },
    { "mediumpurple", SkColorSetARGB(0xFF, 0x93, 0x70, 0xDB) },
    { "mediumseagreen", SkColorSetARGB(0xFF, 0x3C, 0xB3, 0x71) },
    { "mediumslateblue", SkColorSetARGB(0xFF, 0x7B, 0x68, 0xEE) },
    { "mediumspringgreen", SkColorSetARGB(0xFF, 0x00, 0xFA, 0x9A) },
    { "mediumturquoise", SkColorSetARGB(0xFF, 0x48, 0xD1, 0xCC) },
    { "mediumvioletred", SkColorSetARGB(0xFF, 0xC7, 0x15, 0x85) },
    { "midnightblue", SkColorSetARGB(0xFF, 0x19, 0x19, 0x70) },
    { "mintcream", SkColorSetARGB(0xFF, 0xF5, 0xFF, 0xFA) },
    { "mistyrose", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xE1) },
    { "moccasin", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xB5) },
    { "navajowhite", SkColorSetARGB(0xFF, 0xFF, 0xDE, 0xAD) },
    { "navy", SkColorSetARGB(0xFF, 0x00, 0x00, 0x80) },
    { "oldlace", SkColorSetARGB(0xFF, 0xFD, 0xF5, 0xE6) },
    { "olive", SkColorSetARGB(0xFF, 0x80, 0x80, 0x00) },
    { "olivedrab", SkColorSetARGB(0xFF, 0x6B, 0x8E, 0x23) },
    { "orange", SkColorSetARGB(0xFF, 0xFF, 0xA5, 0x00) },
    { "orangered", SkColorSetARGB(0xFF, 0xFF, 0x45, 0x00) },
    { "orchid", SkColorSetARGB(0xFF, 0xDA, 0x70, 0xD6) },
    { "palegoldenrod", SkColorSetARGB(0xFF, 0xEE, 0xE8, 0xAA) },
    { "palegreen", SkColorSetARGB(0xFF, 0x98, 0xFB, 0x98) },
    { "paleturquoise", SkColorSetARGB(0xFF, 0xAF, 0xEE, 0xEE) },
    { "palevioletred", SkColorSetARGB(0xFF, 0xDB, 0x70, 0x93) },
    { "papayawhip", SkColorSetARGB(0xFF, 0xFF, 0xEF, 0xD5) },
    { "peachpuff", SkColorSetARGB(0xFF, 0xFF, 0xDA, 0xB9) },
    { "peru", SkColorSetARGB(0xFF, 0xCD, 0x85, 0x3F) },
    { "pink", SkColorSetARGB(0xFF, 0xFF, 0xC0, 0xCB) },
    { "plum", SkColorSetARGB(0xFF, 0xDD, 0xA0, 0xDD) },
    { "powderblue", SkColorSetARGB(0xFF, 0xB0, 0xE0, 0xE6) },
    { "purple", SkColorSetARGB(0xFF, 0x80, 0x00, 0x80) },
    { "red", SkColorSetARGB(0xFF, 0xFF, 0x00, 0x00) },
    { "rosybrown", SkColorSetARGB(0xFF, 0xBC, 0x8F, 0x8F) },
    { "royalblue", SkColorSetARGB(0xFF, 0x41, 0x69, 0xE1) },
    { "saddlebrown", SkColorSetARGB(0xFF, 0x8B, 0x45, 0x13) },
    { "salmon", SkColorSetARGB(0xFF, 0xFA, 0x80, 0x72) },
    { "sandybrown", SkColorSetARGB(0xFF, 0xF4, 0xA4, 0x60) },
    { "seagreen", SkColorSetARGB(0xFF, 0x2E, 0x8B, 0x57) },
    { "seashell", SkColorSetARGB(0xFF, 0xFF, 0xF5, 0xEE) },
    { "sienna", SkColorSetARGB(0xFF, 0xA0, 0x52, 0x2D) },
    { "silver", SkColorSetARGB(0xFF, 0xC0, 0xC0, 0xC0) },
    { "skyblue", SkColorSetARGB(0xFF, 0x87, 0xCE, 0xEB) },
    { "slateblue", SkColorSetARGB(0xFF, 0x6A, 0x5A, 0xCD) },
    { "slategray", SkColorSetARGB(0xFF, 0x70, 0x80, 0x90) },
    { "slategrey", SkColorSetARGB(0xFF, 0x70, 0x80, 0x90) },
    { "snow", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xFA) },
    { "springgreen", SkColorSetARGB(0xFF, 0x00, 0xFF, 0x7F) },
    { "steelblue", SkColorSetARGB(0xFF, 0x46, 0x82, 0xB4) },
    { "tan", SkColorSetARGB(0xFF, 0xD2, 0xB4, 0x8C) },
    { "teal", SkColorSetARGB(0xFF, 0x00, 0x80, 0x80) },
    { "thistle", SkColorSetARGB(0xFF, 0xD8, 0xBF, 0xD8) },
    { "tomato", SkColorSetARGB(0xFF, 0xFF, 0x63, 0x47) },
    { "turquoise", SkColorSetARGB(0xFF, 0x40, 0xE0, 0xD0) },
    { "violet", SkColorSetARGB(0xFF, 0xEE, 0x82, 0xEE) },
    { "wheat", SkColorSetARGB(0xFF, 0xF5, 0xDE, 0xB3) },
    { "white", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xFF) },
    { "whitesmoke", SkColorSetARGB(0xFF, 0xF5, 0xF5, 0xF5) },
    { "yellow", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0x00) },
    { "yellowgreen", SkColorSetARGB(0xFF, 0x9A, 0xCD, 0x32) }
};
/**
 * parse color string from expression, support:
 * - #RRGGBB
 * - #RGB
 * - #RRGGBBAA
 * - #RGBA
 * - rgb(r, g, b)
 * - rgba(r, g, b, a)
 * - hsl(h, s, l)
 * - hsla(h, s, l, a)
 * - sementics name
 */

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_string(string &str) {
    if (str[0] == '#') {
        return rgba_from_hex_string(str);
    } else if (str.rfind("rgba", 0) == 0) {
        return rgba_from_rgba_expr(str);
    } else if (str.rfind("rgb", 0) == 0) {
        return rgba_from_rgb_expr(str);
    } else if (str.rfind("hsla", 0) == 0) {
        return rgba_from_hsla_expr(str);
    } else if (str.rfind("hsl", 0) == 0) {
        return rgba_from_hsl_expr(str);
    } else {
        auto iter = semantic_color_map.find(str);

        if (iter != semantic_color_map.end()) {
            return std::make_shared<SkColor4f>(SkColor4f::FromColor(iter->second));
        }
    }

    return nullptr;
};

void W3CSkColorParser::color_mix_with_alpha(SkColor4f &color, double alpha) {
    color.fA = color.fA * alpha;
};

int W3CSkColorParser::to_color_component(int color) {
    if (color < 0) {
        return 0;
    }

    return color > 255 ? 255 : color;
}

float W3CSkColorParser::normalize_deg(string& deg_str) {
    // end with deg
    if (deg_str.find_first_of("deg") == deg_str.size() - 4 || is_number(deg_str)) {
        float deg = fmod(stof(deg_str), 360.0);
        if (deg < 0) deg += 360;

        return deg / 360.0;
    } else if (deg_str.find_first_of("rad") == deg_str.size() - 4) {
        float v = stof(deg_str) / (2 * M_PI);
        v = v - (int) v;

        if (v < 0) v += 1;

        return v;
    }

    return -1;
}

float W3CSkColorParser::normalize_percent(string &per) {
    if (
        (per.find_first_of('%') == per.size() - 1) &&
        is_number(per.substr(0, per.size() - 1))
    ) {
        float pert = stof(per);

        if (pert <= 0) return 0;
        if (pert >= 100) return 1;

        return pert / 100.0;
    }

    return -1;
}

int W3CSkColorParser::h(char c) {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return c - '0';
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      return (c - 'a') + 10;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      return (c - 'A') + 10;
  }
  return 0;
}

float W3CSkColorParser::hue_to_rgb(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t-= 1;
    if(t < 1/6.0) return p + (q - p) * 6 * t;
    if(t < 1/2.0) return q;
    if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;

    return p;
}

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_hex_string(string &str) {
    string hex_color_str = str[0] == '#' ? str.substr(1) : str;
    size_t len = hex_color_str.size();
    SkColor color = SK_ColorBLACK;

    // if char is not in [0-9a-fA-F], just return nullptr
    for (char c : hex_color_str) {
        if (
            !(c >= '0' && c <= '9') &&
            !(c >= 'a' && c <= 'f') &&
            !(c >= 'A' && c <= 'F')
        ) return nullptr;
    }
    
    switch (len) {
        // RRGGBBAA
        case 8: {
            color = SkColorSetARGB(
                // A
                (h(hex_color_str[6]) << 4) + h(hex_color_str[7]),
                // R
                (h(hex_color_str[0]) << 4) + h(hex_color_str[1]),
                // G
                (h(hex_color_str[2]) << 4) + h(hex_color_str[3]),
                // B
                (h(hex_color_str[4]) << 4) + h(hex_color_str[5])
            );
            break;
        };
        // RRGGBB
        case 6: {
            color = SkColorSetARGB(
                // A
                0xFF,
                // R
                (h(hex_color_str[0]) << 4) + h(hex_color_str[1]),
                // G
                (h(hex_color_str[2]) << 4) + h(hex_color_str[3]),
                // B
                (h(hex_color_str[4]) << 4) + h(hex_color_str[5])
            );
            break;
        };
        // RGBA
        case 4: {
            color = SkColorSetARGB(
                // A
                (h(hex_color_str[3]) << 4) + h(hex_color_str[3]),
                // R
                (h(hex_color_str[0]) << 4) + h(hex_color_str[0]),
                // G
                (h(hex_color_str[1]) << 4) + h(hex_color_str[1]),
                // B
                (h(hex_color_str[2]) << 4) + h(hex_color_str[2])
            );
            break;
        };
        // RGB
        case 3: {
            color = SkColorSetARGB(
                // A
                0xFF,
                // R
                (h(hex_color_str[0]) << 4) + h(hex_color_str[0]),
                // G
                (h(hex_color_str[1]) << 4) + h(hex_color_str[1]),
                // B
                (h(hex_color_str[2]) << 4) + h(hex_color_str[2])
            );
            break;
        };
    }

    return std::make_shared<SkColor4f>(SkColor4f::FromColor(color));
};

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_rgba_expr(string &str) {
    node_skia_helpers::FunctionExpression expr = node_skia_helpers::parse_func_str(str);

    if (expr.name == "rgba") {
        size_t params_len = expr.params.size();

        if (params_len < 3) {
            return nullptr;
        }

        return std::make_shared<SkColor4f>(SkColor4f::FromColor(
            SkColorSetARGB(
                to_color_component(
                    params_len >= 4 ? stof(expr.params[3]) * 0xFF : 0xFF
                ),
                to_color_component(stoi(expr.params[0])),
                to_color_component(stoi(expr.params[1])),
                to_color_component(stoi(expr.params[2]))
            )
        ));
    }

    return nullptr;
};

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_rgb_expr(string &str) {
    node_skia_helpers::FunctionExpression expr = node_skia_helpers::parse_func_str(str);

    if (expr.name == "rgb") {
        size_t params_len = expr.params.size();

        if (params_len < 3) {
            return nullptr;
        }

        return std::make_shared<SkColor4f>(SkColor4f::FromColor(
            SkColorSetARGB(
                0XFF,
                to_color_component(stoi(expr.params[0])),
                to_color_component(stoi(expr.params[1])),
                to_color_component(stoi(expr.params[2]))
            )
        ));
    }

    return nullptr;
}

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_hsla_expr(string &str) {
    node_skia_helpers::FunctionExpression expr = node_skia_helpers::parse_func_str(str);

    if (expr.name == "hsla") {
        size_t params_len = expr.params.size();

        if (params_len < 3) {
            return nullptr;
        }

        float r, g, b;
        float h = normalize_deg(expr.params[0]);
        float s = normalize_percent(expr.params[1]);
        float l = normalize_percent(expr.params[2]);
        float a = params_len >= 4 ? stof(expr.params[3]) : 1;

        if (h < 0 || s < 0 || l < 0) {
            return nullptr;
        }

        if (s == 0) {
            r = g = b = l;
        } else {
            float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            float p = 2.0 * l - q;
            r = hue_to_rgb(p, q, h + 1/3.0);
            g = hue_to_rgb(p, q, h);
            b = hue_to_rgb(p, q, h - 1/3.0);
        }
        
        return std::make_shared<SkColor4f>(SkColor4f::FromColor(
            SkColorSetARGB(
                a * 255,
                round(r * 255),
                round(g * 255),
                round(b * 255)
            )
        ));
    }

    return nullptr;
}

shared_ptr<SkColor4f> W3CSkColorParser::rgba_from_hsl_expr(string &str) {
    node_skia_helpers::FunctionExpression expr = node_skia_helpers::parse_func_str(str);

    if (expr.name == "hsl") {
        size_t params_len = expr.params.size();

        if (params_len < 3) {
            return nullptr;
        }

        float r, g, b;
        float h = normalize_deg(expr.params[0]);
        float s = normalize_percent(expr.params[1]);
        float l = normalize_percent(expr.params[2]);

        if (h < 0 || s < 0 || l < 0) {
            return nullptr;
        }

        if (s == 0) {
            r = g = b = l;
        } else {
            float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            float p = 2.0 * l - q;
            r = hue_to_rgb(p, q, h + 1/3.0);
            g = hue_to_rgb(p, q, h);
            b = hue_to_rgb(p, q, h - 1/3.0);
        }

        return std::make_shared<SkColor4f>(SkColor4f::FromColor(
            SkColorSetARGB(
                0xFF,
                round(r * 255),
                round(g * 255),
                round(b * 255)
            )
        ));
    }

    return nullptr;
}
