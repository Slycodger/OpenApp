#pragma once
#include "texture.h"
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
    glm::mat4 projection;

    bool saveRenderBuffer;
    bool saveDepthStencilBuffer;
    UIVector2 viewportSize;

    virtual void camera3DCopyTo(UniqueType* ptr) {}
    virtual void camera3DUpdate() {}
    virtual void camera3DAddedToGlobals() {}
    virtual void camera3DSetParent(UniqueType* ptr) {}
    virtual void camera3DAddedChild(UniqueType* ptr) {}
    virtual void camera3DRemovingChild(size_t index) {}

    void transform3DSetParent(UniqueType* ptr) override {

      camera3DSetParent(ptr);
    }
    void transform3DAddedChild(UniqueType* ptr) override {

      camera3DAddedChild(ptr);
    }
    void transform3DRemovingChild(size_t index) override {

      camera3DRemovingChild(index);
    }

    void transform3DCopyTo(UniqueType* ptr) override {
      Camera3D* cPtr = dynamic_cast<Camera3D*>(ptr);
      if (!cPtr)
        return;



      cPtr->renderBuffer = Texture(viewportSize, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);
      cPtr->depthStencilBuffer = Texture(viewportSize, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

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
      
      
      camera3DCopyTo(ptr);
    }

    void transform3DUpdate() override {
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
      camera3DUpdate();
    }

    void transform3DAddedToGlobals() override {
      addGlobalCamera3D(this);
      camera3DAddedToGlobals();
    }

  public:


    float FOV;
    float aspect;
    float near;
    float far;
    float width;
    float height;

    unsigned char stencilLayers;
    Texture renderBuffer;
    Texture depthStencilBuffer;

    Texture* savedRenderBuffer;
    Texture* savedDepthStencilBuffer;

    Vector4 backgroundColor;

    bool perspective;

    UniqueType* create() override {
      return new Camera3D();
    }

    void active() {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glViewport(0, 0, viewportSize.x, viewportSize.y);
    }

    void updateTextureFiltering(unsigned int Filtering) {
      if (Filtering == 0)
        Filtering = renderBuffer.getFiltering();

      renderBuffer.setFiltering(Filtering);
      depthStencilBuffer.setFiltering(Filtering);
      if (savedRenderBuffer)
        savedRenderBuffer->setFiltering(Filtering);
      if (savedDepthStencilBuffer)
        savedDepthStencilBuffer->setFiltering(Filtering);
    }

    UIVector2 getSize() {
      return viewportSize;
    }

    void resize(UIVector2 newSize, bool instant) {
      if (newSize == 0)
        newSize = viewportSize;

      if (instant) {
        unsigned int tFBO = FBO;
        unsigned int tR = renderBuffer;
        unsigned int tDS = depthStencilBuffer;

        renderBuffer.resize(newSize, false);
        depthStencilBuffer.resize(newSize, false);
        if (savedRenderBuffer)
          savedRenderBuffer->resize(newSize, true);
        if (savedDepthStencilBuffer)
          savedDepthStencilBuffer->resize(newSize, true);

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBlitNamedFramebuffer(tFBO, FBO, 0, 0, viewportSize.x, viewportSize.y, 0, 0, newSize.x, newSize.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

        glDeleteFramebuffers(1, &tFBO);
        glDeleteTextures(1, &tR);
        glDeleteTextures(1, &tDS);
      } else {
        renderBuffer.resize(newSize, true);
        depthStencilBuffer.resize(newSize, true);
        if (savedRenderBuffer)
          savedRenderBuffer->resize(newSize, true);
        if (savedDepthStencilBuffer)
          savedDepthStencilBuffer->resize(newSize, true);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
      viewportSize = newSize;
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

    void updateProjection() {
      if (perspective)
        projection = glm::perspective(FOV * _degToRadF, aspect, near, far);
      else
        projection = glm::ortho(-width, width, -height, height, near, far);
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
      savedRenderBuffer = new Texture(viewportSize, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT);

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
      savedDepthStencilBuffer = new Texture(viewportSize, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
    }



    //default
    Camera3D() : Transform3D(), perspective(true), width(0), height(0), camera3DIndex(-1), FOV(), aspect(), near(), far(), stencilLayers(), viewportSize(), 
      saveRenderBuffer(false), saveDepthStencilBuffer(false), savedDepthStencilBuffer(nullptr), savedRenderBuffer(nullptr), FBO(), projection() {}


    //default with self contained
    Camera3D(bool sF) : Transform3D(sF), perspective(true), width(0), height(0), camera3DIndex(-1), FOV(), aspect(), near(), far(), viewportSize(),
      stencilLayers(), saveRenderBuffer(false), saveDepthStencilBuffer(false), FBO(), projection(), savedRenderBuffer(nullptr), savedDepthStencilBuffer(nullptr) {}

    //perspective
    Camera3D(float fov, float Aspect, float N, float F, unsigned char Stencil, UIVector2 vP, Vector4 bg) : Transform3D(), perspective(true), width(0), height(0), camera3DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F), viewportSize(vP),
      stencilLayers(Stencil), saveRenderBuffer(false), saveDepthStencilBuffer(false),
      renderBuffer(vP, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT), savedRenderBuffer(nullptr),
      depthStencilBuffer(vP, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),
      savedDepthStencilBuffer(nullptr), backgroundColor(bg) {

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      projection = glm::perspective(fov * _degToRadF, aspect, near, far);
      addGlobalCamera3D(this);
    }

    //perspective with self contained
    Camera3D(float fov, float Aspect, float N, float F, unsigned int Stencil, UIVector2 vP, Vector4 bg, bool sF) : Transform3D(sF), perspective(true), camera3DIndex(-1), FOV(fov), aspect(Aspect), near(N), far(F), viewportSize(vP),
      stencilLayers(Stencil), saveRenderBuffer(false), saveDepthStencilBuffer(false),
      renderBuffer(vP, GL_LINEAR, GL_RGBA, GL_RGBA, GL_FLOAT), savedRenderBuffer(nullptr),
      depthStencilBuffer(vP, GL_LINEAR, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8),
      savedDepthStencilBuffer(nullptr), backgroundColor(bg) {

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderBuffer, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      projection = glm::perspective(fov * _degToRadF, aspect, near, far);
      addGlobalCamera3D(this);
    }

    ~Camera3D() override {
      removeGlobalCamera3D(this);
      if (savedRenderBuffer)
        delete(savedRenderBuffer);
      if (savedDepthStencilBuffer)
        delete(savedDepthStencilBuffer);
    }




    //Static stuff
    //--------------------------------------------------
    static Camera3D* mainCamera3D;
    static size_t globalCamera3DCount;
    static StaticList<Camera3D*> globalCamera3DInstances;



    //--------------------------------------------------
    static Camera3D* getMainCamera3DIndex() {
      return mainCamera3D;
    }



    //--------------------------------------------------
    static Camera3D* getMainCamera3D() {
      return mainCamera3D;
    }



    //--------------------------------------------------
    static void setMainCamera3D(Camera3D* cam) {
      if (!cam || cam->camera3DIndex >= (size_t)-1) {
        mainCamera3D = nullptr;
        return;
      }
      mainCamera3D = cam;
    }



    //--------------------------------------------------
    static size_t getGlobalCamera3DCount() {
      return globalCamera3DCount;
    }



    //--------------------------------------------------
    static void addGlobalCamera3D(Camera3D* cam) {
      cam->camera3DIndex = globalCamera3DInstances.addItem(cam);
      if (cam->camera3DIndex >= (size_t)-1)
        return;
      globalCamera3DCount++;
    }



    //--------------------------------------------------
    static void removeGlobalCamera3D(Camera3D* cam) {
      if (!cam || cam->camera3DIndex >= (size_t)-1 || !globalCamera3DInstances.removeAt(cam->camera3DIndex))
        return;
      cam->camera3DIndex = -1;
      globalCamera3DCount--;
    }
  };
}