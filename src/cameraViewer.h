#pragma once
#include "camera3D.h"
#include "staticList.h"

namespace openApp {
  class CameraViewer : UniqueType {
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

      if (dynamic_cast<Camera3D*>(ptr)) {

      }

      cameraViewerAddedChild(ptr);
    }

    void uniqueTypeRemovingChild(size_t index) override {

      cameraViewerRemovingChild(index);
    }


    StaticList<UniqueType*> cameraDisplayers;

  public:
    UniqueType* create() override {
      return new CameraViewer();
    }

    CameraViewer() {}

  };
}