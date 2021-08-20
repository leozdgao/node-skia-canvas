#include "modules/skparagraph/include/FontCollection.h"

using skia::textlayout::FontCollection;

// work for class static constructor: https://stackoverflow.com/questions/1197106/static-constructors-in-c-i-need-to-initialize-private-static-objects
class FontCollectionPriv {
public:
  FontCollectionPriv() {
    collection->setDefaultFontManager(SkFontMgr::RefDefault());
  }

  sk_sp<FontCollection> getCollection() { return sk_sp(collection); }
  FontCollection* operator-> () { return collection; }
private:
  FontCollection* collection = new FontCollection();
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
