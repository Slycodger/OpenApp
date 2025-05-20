#include "images.h"
#include "openglBasics.h"
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
using namespace openApp;

std::map<std::string, Image*> loadedImages = std::map<std::string, Image*>();


namespace openApp {
  namespace image {
    Image* loadImage(const char* filePath, bool flipOnLoad, int format, int minChannels, int& returnCode) {
      if (loadedImages.contains(filePath))
        return loadedImages[filePath];

      stbi_set_flip_vertically_on_load(flipOnLoad);

      int x;
      int y;
      int channels;
      unsigned char* buffer = 0;
      buffer = stbi_load(filePath, &x, &y, &channels, minChannels);
      if (!buffer) {
        returnCode = 1;
        loadedImages.insert({ filePath, nullptr });
        return nullptr;
      }

      Image* ret = new Image();
      ret->size = UIVector2(x, y);
      ret->buffer = buffer;
      ret->format = format;
      ret->pathTo = filePath;
      loadedImages.insert({ filePath, ret });
      returnCode = 0;
      return ret;
    }

    void deleteImage(const char* filePath) {
      if (!loadedImages.contains(filePath))
        return;
      Image* img = loadedImages[filePath];
      if (img)
        delete(img);
      loadedImages.erase(filePath);
    }

    void end() {
      for (std::pair<std::string, Image*> p : loadedImages) {
        if (p.second)
          delete(p.second);
      }
      loadedImages.clear();
    }
  }
}