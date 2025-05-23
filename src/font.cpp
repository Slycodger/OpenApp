#include "font.h"
#include "shaderHandling.h"
using namespace openApp;

FT_Library lib;

std::map<std::string, Font*> loadedFonts = std::map<std::string, Font*>();


unsigned int charFBO;

unsigned int charVAO;
unsigned int charVBO;

float squareVerts[] = {
  -1.f, -1.f,     0.f, 1.f,
  1.f, -1.f,      1.f, 1.f,
  -1.f, 1.f,      0.f, 0.f,

  1.f, 1.f,       1.f, 0.f,
  -1.f, 1.f,      0.f, 0.f,
  1.f, -1.f,      1.f, 1.f
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
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenFramebuffers(1, &charFBO);
    }


    Font* getLoadedFont(std::string font) {
      if (!loadedFonts.contains(font))
        return nullptr;
      return loadedFonts[font];
    }

    Font* loadFont(const char* font, std::string name) {
      if (loadedFonts.contains(name))
        return nullptr;

      if (!program::fileExists(font))
        return nullptr;
      FT_Face face;


      if (!FT_New_Face(lib, font, 0, &face)) {
        Font* f = new Font(16, face);
        loadedFonts.insert(std::pair<std::string, Font*>(name, f));
        return f;
      }
      return nullptr;
    }



    std::string sanitizeText(std::string message, float width, float unit, unsigned int fontSize, FT_Face face, Font* font, unsigned int filter) {
      std::string ret;
      std::string word;

      bool wholeLine = true;
      float wordLength = 0;
      float pen = -1;
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      float bC[] = {0, 0, 0, 0};
      for (char c : message) {
        CharContainer& Char = font->loadedCharacters[c];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r' && !Char.texture) {
          unsigned int tex;
          glGenTextures(1, &tex);
          glBindTexture(GL_TEXTURE_2D, tex);
          if (!FT_Load_Char(face, c, FT_LOAD_RENDER))
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
          else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, invalidChar);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bC);

          Char.texture = tex;
          Char.bearing = Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top) * 2 / fontSize;
          Char.size = Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows) * 2 / fontSize;
          Char.advance = (float)(face->glyph->advance.x >> 6) * 2 / fontSize;
        }

        if (c == '\n') {
          pen = -1;
          ret += word;
          wholeLine = true;
          ret += '\n';
          word = "";
          wordLength = 0;
          continue;
        }

         if (pen >= 1 - ((Char.size.x + Char.bearing.x) * unit)) {
          if (wholeLine) {
            ret += word;
            word = "";
            wordLength = 0;
          }
          ret += '\n';
          pen = -1 + wordLength;
          pen += Char.advance * unit;
          wordLength += Char.advance * unit;
          wholeLine = true;
         } else {
           pen += Char.advance * unit;
           wordLength += Char.advance * unit;
         }

         if (c != ' ' && c != '\t' && c != '\r')
           word += c;
         else {
           wholeLine = false;
           ret += word;
           word = "";
           wordLength = 0;
         }

      }
      ret += word;
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      return ret;
    }

    Texture* createTextTexture(std::string message, Font* font, Vector4 fontColor, float width, float aspect, float fontFill, float lineHeight, bool smooth, float cutoff) {
      if (!font)
        return nullptr;
      FT_Face face = font->face;
      unsigned int fontSize = font->getFontSize();

      if (fontFill == 0)
        fontFill = fontSize;

      unsigned int filter = GL_LINEAR;
      if (!smooth)
        filter = GL_NEAREST;


      float height = width / aspect / program::SCREEN_ASPECT;
      Vector2 size = {width, height};
      Vector2 unit = Vector2(fontFill) / size;

      FT_Set_Pixel_Sizes(face, 0, fontSize);
      std::string sanitized = sanitizeText(message, width, unit.x, fontSize, face, font, filter);
      charShader.active();
      charShader.setVector4("color", fontColor);
      charShader.setFloat("cutoff", cutoff);

      glBindFramebuffer(GL_FRAMEBUFFER, charFBO);
      glBindVertexArray(charVAO);
      glBindBuffer(GL_ARRAY_BUFFER, charVBO);
      Texture* retTex = new Texture(UIVector2(size.x, size.y), filter, GL_RGBA, GL_RGBA, GL_FLOAT);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *retTex, 0);
      glViewport(0, 0, width, height);

      float lineSize = lineHeight * fontFill * 2 / height;
      Vector2 pen = Vector2(-1, 1 - lineSize);
      for (char c : sanitized) {
        CharContainer Char = font->loadedCharacters[c];
        Char.bearing = Char.bearing * unit;
        Char.advance = Char.advance * unit.x;
        Char.size = Char.size * unit;

        if (c == '\n') {
          pen.y -= lineSize;
          pen.x = -1;
          continue;
        }
        if (c == ' ' || c == '\t' || c == '\r') {
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
            l, b,           0.f, 1.f,
            r, b,           1.f, 1.f,
            l, t,           0.f, 0.f,

            r, t,           1.f, 0.f,
            l, t,           0.f, 0.f,
            r, b,           1.f, 1.f
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newBuffer), newBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        pen.x += Char.advance;
      }
      glBindTexture(GL_TEXTURE_2D, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glViewport(0, 0, program::SCREEN_SIZE_X, program::SCREEN_SIZE_Y);

      return retTex;
    }


    void end() {
      for (auto p : loadedFonts) {
        if (p.second)
          delete(p.second);
      }
      loadedFonts.clear();

      glDeleteBuffers(1, &charVBO);
      glDeleteVertexArrays(1, &charVAO);
      glDeleteFramebuffers(1, &charFBO);

      FT_Done_FreeType(lib);
    }
  }
}