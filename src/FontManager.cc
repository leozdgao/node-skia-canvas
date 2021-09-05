#include "modules/skparagraph/include/TypefaceFontProvider.h"

#include "FontManager.h"

using skia::textlayout::TypefaceFontProvider;

FontCollectionPriv FontManager::collection;
vector<LocalFontRegistry> FontManager::fonts = {};

Napi::Object FontManager::Init(Napi::Env env, Napi::Object exports) {
  exports.Set("registerFont", Napi::Function::New<RegisterFont>(env, "registerFont"));

  return exports;
}

void FontManager::RegisterFont(const Napi::CallbackInfo& info) {
  Napi::String path = info[0].As<Napi::String>();
  Napi::Object options = info[1].As<Napi::Object>();

  sk_sp<SkTypeface> typeface = SkTypeface::MakeFromFile(path.Utf8Value().data());
    
  if (typeface == nullptr) {
    Napi::Error err = Napi::Error::New(info.Env(), "Invalid font file");
    err.ThrowAsJavaScriptException();

    return;
  }

  string alias;

  if (options.IsObject() && options.Get("family").IsString()) {
    Napi::String family = options.Get("family").As<Napi::String>();
    alias = family.Utf8Value();
  }

  fonts.push_back({ .typeface = typeface, .alias = alias });

  TypefaceFontProvider* provider = new TypefaceFontProvider();
  for (auto i = fonts.begin(), l = fonts.end(); i < l; i++) {
    if (i->alias.empty()) {
      provider->registerTypeface(i->typeface);
    } else {
      provider->registerTypeface(i->typeface, SkString(i->alias));
    }
  }

  collection->setAssetFontManager(sk_sp(provider));
}
