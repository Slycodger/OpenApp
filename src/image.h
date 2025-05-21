#pragma once
#include "vector.h"

namespace openApp {
  struct Image {
    UIVector2 size;
    int format;
    unsigned char* buffer;
    std::string pathTo;

    Image() : size(), format(0), buffer(0), pathTo() {}
    Image(UIVector2 s, int f, unsigned char* b, std::string pathT) : size(s), format(f), buffer(b), pathTo(pathT) {}
    ~Image() {
      if (buffer)
        free(buffer);
    }
  };

  namespace image {
    Image* loadImage(const char* filePath, bool flipOnLoad, int format, int minChannels, int& returnCode);
    void deleteImage(const char* filePath);
    void end();
  }
}