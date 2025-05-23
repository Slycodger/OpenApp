#pragma once

namespace openApp {
  namespace program {
    void drawScreenQuad();
    void defaultView();

    bool fileExists(const char* f);

    unsigned int getShaderMaterialUBO();
    unsigned int getShader2D();
    unsigned int getShader3D();
    unsigned int getGlobalVAO();


    extern unsigned int SCREEN_SIZE_X;
    extern unsigned int SCREEN_SIZE_Y;
    extern float SCREEN_ASPECT;
    extern double DELTA_TIME;
    extern double APP_TIME;
  }
}

#define _SCREEN_SIZE { openApp::program::SCREEN_SIZE_X, openApp::program::SCREEN_SIZE_Y }