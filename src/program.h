#pragma once

namespace openApp {
  namespace program {
    void drawScreenQuad();
    void defaultView();

    unsigned int getShaderMaterialUBO();
    Shader getShader2D();
    Shader getShader3D();
    unsigned int getGlobalVAO();
  }
}