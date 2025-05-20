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
    size_t camera3DIndex;

  protected:
    unsigned int FBO;
    float FOV;
    float aspect;
    float near;
    float far;
    glm::mat4 projection;

    void camera3DCopyTo(UniqueType* ptr) {}
    void transform3DCopyTo(UniqueType* ptr) override {
      Camera3D* cPtr = dynamic_cast<Camera3D*>(ptr);
      if (!cPtr)
        return;



      cPtr->renderBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);
      cPtr->savedRenderBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);
      cPtr->depthStencilBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
      cPtr->savedDepthStencilBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),

      glGenFramebuffers(1, &cPtr->FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, cPtr->FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);



      cPtr->FOV = FOV;
      cPtr->aspect = aspect;
      cPtr->near = near;
      cPtr->far = far;
      cPtr->projection = glm::perspective(FOV * _degToRadF, aspect, near, far);
      cPtr->saveRenderBuffer = saveRenderBuffer;
      cPtr->saveDepthStencilBuffer = saveDepthStencilBuffer;
      cPtr->backgroundColor = backgroundColor;
      
      
      camera3DCopyTo(ptr);
    }

  public:
    virtual void camera3DUpdate() {}
    virtual void camera3DAddedToGlobals() {}


    unsigned char stencilLayers;
    Texture renderBuffer;
    Texture depthStencilBuffer;

    Texture savedRenderBuffer;
    Texture savedDepthStencilBuffer;

    Vector4 backgroundColor;

    bool saveRenderBuffer;
    bool saveDepthStencilBuffer;

    UniqueType* create() override {
      return new Camera3D();
    }

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
      addGlobalCamera(this);
      camera3DAddedToGlobals();
    }


    Camera3D() : Transform3D(), camera3DIndex(-1), FOV(), aspect(), near(), far(), stencilLayers(), saveRenderBuffer(false), saveDepthStencilBuffer(false), FBO(), projection() {}
    Camera3D(bool sF) : Transform3D(sF), camera3DIndex(-1), FOV(), aspect(), near(), far(), stencilLayers(), saveRenderBuffer(false), saveDepthStencilBuffer(false), FBO(), projection() {}
    Camera3D(float fov, float Aspect, float N, float F, unsigned char Stencil, Vector4 bg) : Transform3D(), camera3DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F),
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
      addGlobalCamera(this);
    }
    Camera3D(float fov, float Aspect, float N, float F, unsigned int Stencil, Vector4 bg, bool sF) : Transform3D(sF), camera3DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F),
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
      addGlobalCamera(this);
    }
    ~Camera3D() override {
      removeGlobalCamera(this);
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
      if (!cam || cam->camera3DIndex >= (size_t)-1) {
        mainCamera = nullptr;
        return;
      }
      mainCamera = cam;
    }



    //--------------------------------------------------
    static size_t getGlobalCameraCount() {
      return globalCamera3DCount;
    }



    //--------------------------------------------------
    static void addGlobalCamera(Camera3D* cam) {
      cam->camera3DIndex = globalCamera3DInstances.addItem(cam);
      if (cam->camera3DIndex >= (size_t)-1)
        return;
      globalCamera3DCount++;
    }



    //--------------------------------------------------
    static void removeGlobalCamera(Camera3D* cam) {
      if (!cam || cam->camera3DIndex >= (size_t)-1 || !globalCamera3DInstances.removeAt(cam->camera3DIndex))
        return;
      cam->camera3DIndex = -1;
      globalCamera3DCount--;
    }
  };
}