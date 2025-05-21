#pragma once
#include "visual3D.h"

namespace openApp {
  namespace modelLoading {
    UniqueType* loadModel(std::string path);
    void end();
  }
}