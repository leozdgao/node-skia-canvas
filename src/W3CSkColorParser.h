#ifndef NODE_SKIA_COLOR_H
#define NODE_SKIA_COLOR_H

#include <string>
#include <memory>
#include <unordered_map>
#include <include/core/SkColor.h>

using std::string;
using std::unordered_map;
using std::shared_ptr;
using SkColorMap = unordered_map<string, SkColor>;

class W3CSkColorParser {
private:
    static const SkColorMap semantic_color_map;
    
    static int h(char c);
    static int to_color_component(int val);
    static float hue_to_rgb(float t1, float t2, float hue);
    static float normalize_deg(string &deg);
    static float normalize_percent(string &per);
    static shared_ptr<SkColor4f> rgba_from_hex_string(string &str);
    static shared_ptr<SkColor4f> rgba_from_rgb_expr(string &str);
    static shared_ptr<SkColor4f> rgba_from_rgba_expr(string &str);
    static shared_ptr<SkColor4f> rgba_from_hsl_expr(string &str);
    static shared_ptr<SkColor4f> rgba_from_hsla_expr(string &str);

public:
    static void color_mix_with_alpha(SkColor4f &color, double alpha);
    static shared_ptr<SkColor4f> rgba_from_string(string &str);
};

#endif
