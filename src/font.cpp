#include "font.h"
#include "program.h"
#include "shaderHandling.h"
#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H
using namespace openApp;

FT_Library lib;

std::map<std::string, std::pair<FT_Face, Font*>> loadedFonts = std::map<std::string, std::pair<FT_Face, Font*>>();


unsigned int charFBO;

unsigned int charVAO;
unsigned int charVBO;

float squareVerts[] = {
  -1.f, -1.f, 0.f, 0.f,
  1.f, -1.f, 1.f, 0.f,
  -1.f, 1.f, 0.f, 1.f,

  1.f, 1.f, 1.f, 1.f,
  -1.f, 1.f, 0.f, 1.f,
  1.f, -1.f, 1.f, 0.f
};

unsigned char invalidChar[] = { 
    0,    0,  255,  255,  255,    0,    0,    0,
    0,  255,    0,    0,    0,  255,    0,    0,
  255,    0,    0,    0,    0,  255,    0,    0, 
    0,    0,    0,    0,    0,  255,    0,    0,
    0,    0,    0,  255,  255,    0,    0,    0, 
    0,    0,    0,  255,    0,    0,    0,    0, 
    0,    0,    0,    0,    0,    0,    0,    0, 
    0,    0,    0,  255,    0,    0,    0,    0
};

Shader charShader;


namespace openApp {
  namespace font {
    void start() {
      FT_Init_FreeType(&lib);



      const char* vertexShaders[] = {
        "./shaders/viewportShader.vert"
      };
      const char* fragmentShaders[] = {
        "./shaders/charShader.frag"
      };

      ShaderPair shaderPair[2] = {ShaderPair(vertexShaders, 1, GL_VERTEX_SHADER), ShaderPair(fragmentShaders, 1, GL_FRAGMENT_SHADER)};
      charShader = Shader::createShader("", shaderPair, 2);


      glGenVertexArrays(1, &charVAO);
      glBindVertexArray(charVAO);
      glGenBuffers(1, &charVBO);
      glBindBuffer(GL_ARRAY_BUFFER, charVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerts), squareVerts, GL_DYNAMIC_DRAW);
      
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
      glEnableVertexAttribArray(1);

      glBindVertexArray(0);

      glGenFramebuffers(1, &charFBO);
    }


    Font* getLoadedFont(std::string font) {
      if (!loadedFonts.contains(font))
        return nullptr;
      return loadedFonts[font].second;
    }

    void loadFont(const char* font, std::string name) {
      if (loadedFonts.contains(name))
        return;

      if (!program::fileExists(font))
        return;
      FT_Face face;

      if (!FT_New_Face(lib, font, 0, &face))
        loadedFonts.insert(std::pair<std::string, std::pair<FT_Face, Font*>>(name, {face, nullptr}));
    }


    
    std::string sanitizeText(std::string message, float width, FT_Face face, Font* font) {
      std::string ret;
      std::string word;

      bool wholeLine = true;
      float aWidth = width - width / 2;
      float pen = -aWidth;
      for (char c : message) {
        CharContainer& Char = font->loadedCharacters[c];
        if (!Char.texture) {
          unsigned int tex;
          glGenTextures(1, &tex);
          glBindTexture(GL_TEXTURE_2D, tex);
          if (!FT_Load_Char(face, c, FT_LOAD_RENDER))
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
          else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, invalidChar);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

          Char.texture = tex;
          Char.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
          Char.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
          Char.advance = face->glyph->advance.x >> 6;
        }

        if (c != ' ' && c != '\n' && c != '\t' && c != '\r')
          word += c;
        else {
          wholeLine = false;
          ret += word;
          word = "";
        }

        pen += Char.bearing.x;
        if (pen >= aWidth - Char.size.x) {
          if (wholeLine) {
            ret += word;
            word = "";
          }
          ret += '\n';
          pen = -aWidth;
        } else
          pen += Char.advance;
      }
      ret += word;

      return ret;
    }

    Texture* createTextTexture(std::string message, std::string fontTU, Vector4 fontColor, UIVector2 texSize, unsigned int fontSize, float lineHeight) {
      if (!loadedFonts.contains(fontTU))
        return nullptr;
      FT_Face face = loadedFonts[fontTU].first;
      Font* font = loadedFonts[fontTU].second;
      if (!font)
        font = new Font();

      FT_Set_Pixel_Sizes(face, 0, fontSize);
      sanitizeText(message, texSize.x, face, font);
      charShader.active();
      charShader.setVector4("color", fontColor);

      glBindFramebuffer(GL_FRAMEBUFFER, charFBO);
      glBindVertexArray(charVAO);
      Texture* retTex = new Texture(texSize, GL_NEAREST, GL_RGBA, GL_RGBA, GL_FLOAT);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *retTex, 0);
      glViewport(0, 0, texSize.x, texSize.y);

      float lineSize = (fontSize * lineHeight) / texSize.y;
      Vector2 pen = Vector2(-1, 1 - lineSize);
      for (char c : message) {
        CharContainer& Char = font->loadedCharacters[c];

        if (c == '\n') {
          pen.y += lineSize;
          continue;
        }
        if (c == ' ' || c == '\t') {
          pen.x += Char.advance;
          continue;
        }

        glBindTexture(GL_TEXTURE_2D, Char.texture);
        glActiveTexture(GL_TEXTURE0);


        float t = pen.y + Char.bearing.y;
        float b = pen.y + Char.bearing.y - Char.size.y;
        float l = pen.x + Char.bearing.x;
        float r = pen.x + Char.bearing.x + Char.size.x;
        float newBuffer[] = {
            l, b,           0.f, 0.f,
            r, b,           1.f, 0.f,
            l, t,           0.f, 1.f,

            r, t,           1.f, 1.f,
            l, t,           0.f, 1.f,
            r, b,           1.f, 0.f
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newBuffer), newBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        pen.x += Char.advance;
      }
      glBindTexture(GL_TEXTURE_2D, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      return retTex;
    }


    void end() {
      for (auto p : loadedFonts) {
        if (p.second.second)
          delete(p.second.second);
        FT_Done_Face(p.second.first);
      }
      loadedFonts.clear();

      FT_Done_FreeType(lib);
    }
  }
}