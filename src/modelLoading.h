#pragma once
#include "visual3D.h"

namespace openApp {
  namespace modelLoading {
    Visual3D* loadModel(std::string path);
    void end();
  }
}