#pragma once
#include "vector.h"
#include "openglBasics.h"
#include "image.h"

namespace openApp {
  class Texture {
    unsigned int internal;
    UIVector2 size;
    unsigned int filtering;
    unsigned int internalFormat;
    unsigned int format;
    unsigned int type;
    Image* internalImage;

  public:

    unsigned int getInternal() const {
      return internal;
    }
    unsigned int getFiltering() const {
      return filtering;
    }
    UIVector2 getSize() const {
      return size;
    }

    void setFiltering(unsigned int f) {
      glTextureParameteri(internal, GL_TEXTURE_MIN_FILTER, f);
      glTextureParameteri(internal, GL_TEXTURE_MAG_FILTER, f);
      filtering = f;
    }

    Texture(UIVector2 Size, unsigned int Filtering, unsigned int InternalFormat, unsigned int Format, unsigned int Type) : internal(0), size(Size), filtering(Filtering), internalFormat(InternalFormat), format(Format), type(Type), internalImage(0) {
      glGenTextures(1, &internal);
      glBindTexture(GL_TEXTURE_2D, internal);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, 0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    Texture(Image* img, unsigned int Filtering, unsigned int Format, unsigned int Type) : internal(0), size(), internalFormat(0), filtering(Filtering), format(Format), type(Type), internalImage(img) {
      if (!img)
        return;
      size = img->size;
      internalFormat = img->format;

      glGenTextures(1, &internal);
      glBindTexture(GL_TEXTURE_2D, internal);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, img->buffer);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    Texture() : internal(0), size(), internalFormat(0), filtering(0), format(0), type(0), internalImage(0) {}


    operator unsigned int() {
      return internal;
    }
  };
}