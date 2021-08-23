#pragma once

#include <string>
#include <vector>
#include <napi.h>

#include "include/core/SkTypeface.h"
#include "modules/skparagraph/include/FontCollection.h"

using std::string;
using std::vector;

using skia::textlayout::FontCollection;

struct LocalFontRegistry {
  sk_sp<SkTypeface> typeface;
  string alias;
};

// work for class static constructor:
// @link https://stackoverflow.com/questions/1197106/static-constructors-in-c-i-need-to-initialize-private-static-objects
class FontCollectionPriv {
public:
  FontCollectionPriv() {
    collection->setDefaultFontManager(SkFontMgr::RefDefault());
  }

  sk_sp<FontCollection> getCollection() { 
    if (sp_collection == nullptr) {
      sp_collection = sk_sp(collection);
    }
    
    return sp_collection;
  }
  FontCollection* operator-> () { return collection; }
private:
  FontCollection* collection = new FontCollection();
  sk_sp<FontCollection> sp_collection = nullptr;
};

class FontManager {
public:  
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static void RegisterFont(const Napi::CallbackInfo& info);

  static FontCollectionPriv collection;
  static sk_sp<FontCollection> getCollection();
  static vector<LocalFontRegistry> fonts;

private:
  FontManager();
  ~FontManager();
};
