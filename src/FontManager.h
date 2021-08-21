#pragma once

#include "modules/skparagraph/include/FontCollection.h"

using skia::textlayout::FontCollection;

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
private:
  FontManager();
  ~FontManager();
public:
  static FontCollectionPriv collection;
  static sk_sp<FontCollection> getCollection();
  static void addTypeFace();
};
