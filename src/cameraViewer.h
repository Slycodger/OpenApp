#pragma once
#include "camera3D.h"
#include "camera2D.h"
#include "staticList.h"
#include "visual2D.h"

namespace openApp {
  class CameraViewer : public UniqueType {
  protected :
    virtual void cameraViewerCopyTo(UniqueType* ptr) {}
    virtual void cameraViewerUpdate() {}
    virtual void cameraViewerAddedToGlobals() {}
    virtual void cameraViewerSetParent(UniqueType* ptr) {}
    virtual void cameraViewerAddedChild(UniqueType* ptr) {}
    virtual void cameraViewerRemovingChild(size_t index) {}

    void copyTo(UniqueType* ptr) override {

      cameraViewerCopyTo(ptr);
    }

    void uniqueTypeUpdate() override {

      cameraViewerUpdate();
    }

    void uniqueTypeAddedToGlobals() override {

      cameraViewerAddedToGlobals();
    }

    void uniqueTypeSetParent(UniqueType* ptr) override {

      cameraViewerSetParent(ptr);
    }

    void uniqueTypeAddedChild(UniqueType* ptr) override {

      Camera3D* cam3D = dynamic_cast<Camera3D*>(ptr);
      if (cam3D) {
        Visual2D* nVisual = new Visual2D(Mesh2D::getGlobalMesh2D("square"), 0, stencil);

        cam3D->renderBufferSaving(true);
        nVisual->srcMaterial.albedoTexture = cam3D->savedRenderBuffer;
        cameraDisplayers.addItem(ptr);
        nVisual->setParent(this);
      } else {
        Camera2D* cam2D = dynamic_cast<Camera2D*>(ptr);
        if (cam2D) {
          Visual2D* nVisual = new Visual2D(Mesh2D::getGlobalMesh2D("square"), 0, stencil);

          cam2D->renderBufferSaving(true);
          nVisual->srcMaterial.albedoTexture = cam2D->savedRenderBuffer;
          cameraDisplayers.addItem(ptr);
          nVisual->setParent(this);
        }
      }

      cameraViewerAddedChild(ptr);
    }

    void uniqueTypeRemovingChild(size_t index) override {

      cameraViewerRemovingChild(index);
    }


    StaticList<UniqueType*> cameraDisplayers;

    unsigned char stencil;

  public:
    UniqueType* create() override {
      return new CameraViewer();
    }

    unsigned char getStencil() {
      return stencil;
    }
    void setStencil(unsigned char nS) {
      stencil = nS;
    }



    CameraViewer() : UniqueType(), cameraDisplayers(), stencil(0) {}
    CameraViewer(unsigned char Stencil) : UniqueType(), cameraDisplayers(), stencil(Stencil) {}
    ~CameraViewer() {
      for (UniqueType** u : cameraDisplayers)
        if (*u)
          delete(*u);
      cameraDisplayers.clear();
    }
  };
}