#pragma once

namespace openApp {
  class Program {
    static unsigned int* globalVAO;

  public:
    static unsigned int getGlobalVAO() {
      return *globalVAO;
    }
  };

  namespace program {
    void drawSquare();
    void defaultView();
  }
}