#pragma once
#include "texture.h"
#include <map>

namespace openApp {

  struct CharContainer {
    unsigned int texture;
    IVector2 bearing;
    UIVector2 size;
    float advance;

    CharContainer() : texture(0), bearing(), size(), advance() {}
    CharContainer(unsigned int t, IVector2 b, UIVector2 s, float a) : texture(t), bearing(b), size(s), advance(a) {}
  };

  struct Font {
    std::map<char, CharContainer> loadedCharacters;

    Font() : loadedCharacters() {}
    ~Font() {
      for (auto &i : loadedCharacters)
        glDeleteTextures(1, &i.second.texture);
    }
  };


  namespace font {
    void start();
    Font* getLoadedFont(std::string font);
    void loadFont(const char* font, std::string name);
    Texture* createTextTexture(std::string message, std::string fontTU, Vector4 fontColor, UIVector2 texSize, unsigned int fontSize, float lineHeight);
    void end();
  }
}