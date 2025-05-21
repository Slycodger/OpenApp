#pragma once
#include "texture.h"
#include "globals.h"
#include "transform2D.h"
#include "staticList.h"
#include "shaderHandling.h"
#include "program.h"

namespace openApp {
  class Camera2D : public Transform2D {
    size_t camera2DIndex;

  protected:
    unsigned int FBO;
    float FOV;
    float aspect;
    float near;
    float far;
    glm::mat4 projection;

    bool saveRenderBuffer;
    bool saveDepthStencilBuffer;

    virtual void camera2DCopyTo(UniqueType* ptr) {}
    virtual void camera2DUpdate() {}
    virtual void camera2DAddedToGlobals() {}
    virtual void camera2DSetParent(UniqueType* ptr) {}
    virtual void camera2DAddedChild(UniqueType* ptr) {}
    virtual void camera2DRemovingChild(size_t index) {}

    void transform2DSetParent(UniqueType* ptr) override {

      camera2DSetParent(ptr);
    }
    void transform2DAddedChild(UniqueType* ptr) override {

      camera2DAddedChild(ptr);
    }
    void transform2DRemovingChild(size_t index) override {

      camera2DRemovingChild(index);
    }

    void transform2DCopyTo(UniqueType* ptr) override {
      Camera2D* cPtr = dynamic_cast<Camera2D*>(ptr);
      if (!cPtr)
        return;



      cPtr->renderBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);
      cPtr->depthStencilBuffer = Texture(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

      glGenFramebuffers(1, &cPtr->FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, cPtr->FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      cPtr->renderBufferSaving(saveRenderBuffer);
      cPtr->depthStencilBufferSaving(saveDepthStencilBuffer);

      cPtr->FOV = FOV;
      cPtr->aspect = aspect;
      cPtr->near = near;
      cPtr->far = far;
      cPtr->projection = glm::perspective(FOV * _degToRadF, aspect, near, far);
      cPtr->backgroundColor = backgroundColor;


      camera2DCopyTo(ptr);
    }

    void transform2DUpdate() override {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);

      if (saveRenderBuffer && savedRenderBuffer) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *savedRenderBuffer, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, renderBuffer);
        glActiveTexture(GL_TEXTURE0);

        openApp::program::drawScreenQuad();

        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      }

      if (saveDepthStencilBuffer && savedDepthStencilBuffer) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *savedDepthStencilBuffer, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
        glActiveTexture(GL_TEXTURE0);

        openApp::program::drawScreenQuad();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      }

      glClearStencil(stencilLayers);
      glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      camera2DUpdate();
    }

    void transform2DAddedToGlobals() override {
      addGlobalCamera3D(this);
      camera2DAddedToGlobals();
    }

  public:


    unsigned char stencilLayers;
    Texture renderBuffer;
    Texture depthStencilBuffer;

    Texture* savedRenderBuffer;
    Texture* savedDepthStencilBuffer;

    Vector4 backgroundColor;

    UniqueType* create() override {
      return new Camera2D();
    }

    void active() {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }

    float* getProjection() {
      return glm::value_ptr(projection);
    }

    glm::mat4 getTransformMatrix() override {
      glm::mat4 mat(1);
      mat = glm::rotate(mat, rotation * _degToRadF, glm::vec3(0, 0, 1));
      mat = glm::translate(mat, glm::vec3(-position.x, -position.y, 0));
      return mat;
    }

    bool savingRenderBuffer() {
      return saveRenderBuffer;
    }
    bool savingDepthStencilBuffer() {
      return saveDepthStencilBuffer;
    }
    void renderBufferSaving(bool onOff) {
      saveRenderBuffer = onOff;

      if (!onOff) {
        if (savedRenderBuffer)
          delete(savedRenderBuffer);
        return;
      }
      if (savedRenderBuffer)
        return;
      savedRenderBuffer = new Texture(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);

    }
    void depthStencilBufferSaving(bool onOff) {
      saveDepthStencilBuffer = onOff;

      if (!onOff) {
        if (savedDepthStencilBuffer)
          delete(savedDepthStencilBuffer);
        return;
      }
      if (savedDepthStencilBuffer)
        return;
      savedDepthStencilBuffer = new Texture(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
    }



    Camera2D() : Transform2D(), camera2DIndex(-1), FOV(), aspect(), near(), far(), stencilLayers(),
      saveRenderBuffer(false), saveDepthStencilBuffer(false), savedDepthStencilBuffer(nullptr), savedRenderBuffer(nullptr), FBO(), projection() {
    }

    Camera2D(bool sF) : Transform2D(sF), camera2DIndex(-1), FOV(), aspect(), near(), far(),
      stencilLayers(), saveRenderBuffer(false), saveDepthStencilBuffer(false), FBO(), projection(), savedRenderBuffer(nullptr), savedDepthStencilBuffer(nullptr) {
    }

    Camera2D(float fov, float Aspect, float N, float F, unsigned char Stencil, Vector4 bg) : Transform2D(), camera2DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F),
      stencilLayers(Stencil), saveRenderBuffer(false), saveDepthStencilBuffer(false),
      renderBuffer(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT), savedRenderBuffer(nullptr),
      depthStencilBuffer(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),
      savedDepthStencilBuffer(nullptr), backgroundColor(bg) {

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      projection = glm::perspective(fov * _degToRadF, aspect, near, far);
      addGlobalCamera3D(this);
    }
    Camera2D(float fov, float Aspect, float N, float F, unsigned int Stencil, Vector4 bg, bool sF) : Transform2D(sF), camera2DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F),
      stencilLayers(Stencil), saveRenderBuffer(false), saveDepthStencilBuffer(false),
      renderBuffer(_SCREEN_SIZE, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT), savedRenderBuffer(nullptr),
      depthStencilBuffer(_SCREEN_SIZE, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),
      savedDepthStencilBuffer(nullptr), backgroundColor(bg) {

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      projection = glm::perspective(fov * _degToRadF, aspect, near, far);
      addGlobalCamera3D(this);
    }
    ~Camera2D() override {
      removeGlobalCamera2D(this);
      if (savedRenderBuffer)
        delete(savedRenderBuffer);
      if (savedDepthStencilBuffer)
        delete(savedDepthStencilBuffer);
    }




    //Static stuff
    //--------------------------------------------------
    static Camera2D* mainCamera3D;
    static size_t globalCamera2DCount;
    static StaticList<Camera2D*> globalCamera2DInstances;



    //--------------------------------------------------
    static Camera2D* getMainCamera2DIndex() {
      return mainCamera3D;
    }



    //--------------------------------------------------
    static Camera2D* getMainCamera2D() {
      return mainCamera3D;
    }



    //--------------------------------------------------
    static void setMainCamera2D(Camera2D* cam) {
      if (!cam || cam->camera2DIndex >= (size_t)-1) {
        mainCamera3D = nullptr;
        return;
      }
      mainCamera3D = cam;
    }



    //--------------------------------------------------
    static size_t getGlobalCamera2DCount() {
      return globalCamera2DCount;
    }



    //--------------------------------------------------
    static void addGlobalCamera3D(Camera2D* cam) {
      cam->camera2DIndex = globalCamera2DInstances.addItem(cam);
      if (cam->camera2DIndex >= (size_t)-1)
        return;
      globalCamera2DCount++;
    }



    //--------------------------------------------------
    static void removeGlobalCamera2D(Camera2D* cam) {
      if (!cam || cam->camera2DIndex >= (size_t)-1 || !globalCamera2DInstances.removeAt(cam->camera2DIndex))
        return;
      cam->camera2DIndex = -1;
      globalCamera2DCount--;
    }
  };
}