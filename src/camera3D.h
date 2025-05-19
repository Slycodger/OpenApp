#pragma once
#include "textures.h"
#include "globals.h"
#include <vector>
#include <algorithm>
#include "transform3D.h"
#include "staticList.h"
#include "shaderHandling.h"
#include "program.h"

namespace openApp {
  class Camera3D : public Transform3D {
    unsigned int FBO;
    float FOV;
    float aspect;
    float near;
    float far;
    glm::mat4 projection;
    size_t index;

  public:
    virtual void camera3DUpdate() {}
    virtual void camera3DAddedToGlobals() {}


    unsigned int stencilLayers;
    Texture renderBuffer;
    Texture depthStencilBuffer;

    Texture savedRenderBuffer;
    Texture savedDepthStencilBuffer;

    Vector4 backgroundColor;

    bool saveRenderBuffer;
    bool saveDepthStencilBuffer;

    void active() {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }

    float* getProjection() {
      return glm::value_ptr(projection);
    }

    glm::mat4 getTransformMatrix() override {
      glm::mat4 mat(1);
      mat = glm::rotate(mat, rotation.x * _degToRadF, glm::vec3(1, 0, 0));
      mat = glm::rotate(mat, rotation.y * _degToRadF, glm::vec3(0, 1, 0));
      mat = glm::rotate(mat, rotation.z * _degToRadF, glm::vec3(0, 0, 1));
      mat = glm::translate(mat, glm::vec3(-position.x, -position.y, -position.z));
      return mat;
    }

    void transform3DUpdate() override {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);

      if (saveRenderBuffer) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, savedRenderBuffer, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, renderBuffer);
        glActiveTexture(GL_TEXTURE0);

        openApp::program::drawSquare();

        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      }

      if (saveDepthStencilBuffer) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, savedDepthStencilBuffer, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
        glActiveTexture(GL_TEXTURE0);

        openApp::program::drawSquare();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      }

      glClearStencil(stencilLayers);
      glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      camera3DUpdate();
    }

    void transform3DAddedToGlobals() override {
      addCamera(this);
      camera3DAddedToGlobals();
    }


    Camera3D(float fov, float Aspect, float N, float F, unsigned int Stencil, Vector4 bg) : index(-1), Transform3D(), FOV(fov), aspect(Aspect), near(N), far(F),
      stencilLayers(Stencil), saveRenderBuffer(false), saveDepthStencilBuffer(false),
      renderBuffer(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT), savedRenderBuffer(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT),
      depthStencilBuffer(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),
      savedDepthStencilBuffer(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8), backgroundColor(bg) {

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      projection = glm::perspective(fov * _degToRadF, aspect, near, far);
      addCamera(this);
    }
    ~Camera3D() {
      removeCamera(this);
    }




    //Static stuff
    //--------------------------------------------------
    static Camera3D* mainCamera;
    static size_t globalCamera3DCount;
    static StaticList<Camera3D*> globalCamera3DInstances;



    //--------------------------------------------------
    static Camera3D* getMainCameraIndex() {
      return mainCamera;
    }



    //--------------------------------------------------
    static Camera3D* getMainCamera() {
      return mainCamera;
    }



    //--------------------------------------------------
    static void setMainCamera(Camera3D* cam) {
      if (!cam || cam->index >= (size_t)-1) {
        mainCamera = nullptr;
        return;
      }
      mainCamera = cam;
    }



    //--------------------------------------------------
    static size_t getCameraCount() {
      return globalCamera3DCount;
    }



    //--------------------------------------------------
    static void addCamera(Camera3D* cam) {
      cam->index = globalCamera3DInstances.addItem(cam);
      if (cam->index >= (size_t)-1)
        return;
      globalCamera3DCount++;
    }



    //--------------------------------------------------
    static void removeCamera(Camera3D* cam) {
      if (!cam || cam->index >= (size_t)-1 || !globalCamera3DInstances.removeAt(cam->index))
        return;
      cam->index = -1;
      globalCamera3DCount--;
    }
  };
}