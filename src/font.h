#pragma once
#include "texture.h"
#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

namespace openApp {

  struct CharContainer {
    unsigned int texture;
    Vector2 bearing;
    Vector2 size;
    float advance;

    CharContainer() : texture(0), bearing(), size(), advance() {}
    CharContainer(unsigned int t, Vector2 b, Vector2 s, float a) : texture(t), bearing(b), size(s), advance(a) {}
  };

  class Font {
    unsigned int fontSize;

  public:
    std::map<char, CharContainer> loadedCharacters;
    FT_Face face;

    unsigned int getFontSize() {
      return fontSize;
    }
    void setFontSize(unsigned int size) {
      if (size == fontSize || size == 0)
        return;
      fontSize = size;
      for (auto& p : loadedCharacters)
        glDeleteTextures(1, &p.second.texture);
      loadedCharacters.clear();
    }

    Font() : loadedCharacters(), fontSize(16), face(0) {}
    Font(unsigned int fS, FT_Face f) : loadedCharacters(), fontSize(fS), face(f) {}
    ~Font() {
      for (auto& p : loadedCharacters)
        glDeleteTextures(1, &p.second.texture);
      loadedCharacters.clear();
      FT_Done_Face(face);
    }
  };


  namespace font {
    void start();
    Font* getLoadedFont(std::string font);
    Font* loadFont(const char* font, std::string name);
    Texture* createTextTexture(std::string message, Font* font, Vector4 fontColor, float width, float aspect, float fontFill, float lineHeight, bool smooth, float cutoff);
    void end();
  }
}