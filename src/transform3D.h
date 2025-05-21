#pragma once
#include "uniqueType.h"
#include "vector.h"
#include "openglBasics.h"
#include "constants.h"
#include "staticList.h"
#include <iostream>

namespace openApp {
  class Transform3D : public UniqueType {
  protected:
    virtual void transform3DCopyTo(UniqueType* ptr) {}
    virtual void transform3DUpdate() {}
    virtual void transform3DAddedToGlobals() {}
    virtual void transform3DSetParent(UniqueType* ptr) {}
    virtual void transform3DAddedChild(UniqueType* ptr) {}
    virtual void transform3DRemovingChild(size_t index) {}

    void uniqueTypeSetParent(UniqueType* ptr) override {
      Transform3D* tPtr = dynamic_cast<Transform3D*>(ptr);
      if (!tPtr) {
        relativePosition = Vector3();
        relativeRotation = Vector3();
        relativeScale = Vector3(1);
        relativeParentRotation = Vector3();
        return;
      }

      relativePosition = position - tPtr->position;
      relativeParentRotation = tPtr->rotation;
      relativeScale = scale / tPtr->scale;
      relativeRotation = rotation - tPtr->rotation;

      transform3DSetParent(ptr);
    }

    void uniqueTypeAddedChild(UniqueType* ptr) override {

      transform3DAddedChild(ptr);
    }
    void uniqueTypeRemovingChild(size_t index) override {

      transform3DRemovingChild(index);
    }

    void uniqueTypeUpdate() override {
      if (!parent)
        updateTransformTree();

      transform3DUpdate();
    }
    void uniqueTypeAddedToGlobals() override {
      transform3DAddedToGlobals();
    }


    void copyTo(UniqueType* ptr) override {
      Transform3D* tPtr = dynamic_cast<Transform3D*>(ptr);
      if (!tPtr)
        return;

      tPtr->position = position;
      tPtr->rotation = rotation;
      tPtr->scale = scale;
      tPtr->rotation = rotation;

      transform3DCopyTo(ptr);
    }

  public:

    Vector3 position;
    Vector3 scale;
    Vector3 rotation;
    Vector3 relativePosition;
    Vector3 relativeScale;
    Vector3 relativeRotation;
    Vector3 relativeParentRotation;

    virtual glm::mat4 getTransformMatrix() {
      glm::mat4 mat(1);
      mat = glm::translate(mat, glm::vec3(position.x, position.y, position.z));
      mat = glm::rotate(mat, rotation.z * _degToRadF, glm::vec3(0, 0, 1));
      mat = glm::rotate(mat, rotation.y * _degToRadF, glm::vec3(0, 1, 0));
      mat = glm::rotate(mat, rotation.x * _degToRadF, glm::vec3(1, 0, 0));
      mat = glm::scale(mat, glm::vec3(scale.x, scale.y, scale.z));
      return mat;
    }



    UniqueType* create() override {
      return new Transform3D();
    }

    void updateTransform() {
      if (!parent)
        return;
      Transform3D* parent = dynamic_cast<Transform3D*>(this->parent);
      if (!parent)
        return;

      //Get variables set up
      Vector3 diffRotation = parent->rotation - relativeParentRotation;
      Vector3 relative = relativePosition * parent->scale / relativeScale;
      Vector3 swap;

      float cosx = cos(diffRotation.x * _degToRadF);
      float sinx = sin(diffRotation.x * _degToRadF);

      float cosy = cos(diffRotation.y * _degToRadF);
      float siny = sin(diffRotation.y * _degToRadF);

      float cosz = cos(diffRotation.z * _degToRadF);
      float sinz = sin(diffRotation.z * _degToRadF);


      //Scale first
      scale = relativeScale * parent->scale;

      //Then rotated position
      //Goes in z, y, x order

      //z
      swap.x = relative.x * cosz - relative.y * sinz;
      swap.y = relative.x * sinz + relative.y * cosz;
      relative = swap;

      //y
      swap.x = relative.x * cosy - relative.z * siny;
      swap.z = relative.x * siny + relative.z * cosy;
      relative = swap;

      //x
      position.x = relative.x;
      position.y = relative.y * sinx + relative.y * cosx;
      position.z = relative.z * -cosx - relative.y * -sinx;


      //Move to relative position
      position += parent->position;

      //And set rotation to relative rotation
      rotation = parent->rotation + relativeRotation;
    }

    void updateTransformTree() {
      if (children.size() <= 0)
        return;

      for (UniqueType** childP : children) {
        if (!childP || !*childP)
          continue;
        Transform3D* tPtr = dynamic_cast<Transform3D*>(*childP);
        if (!tPtr)
          continue;

        tPtr->updateTransform();
        tPtr->updateTransformTree();
      }
    }


    Transform3D() : UniqueType(), position(), scale(1), rotation(), relativePosition(), relativeScale(1), relativeRotation(), relativeParentRotation(){}
    Transform3D(bool sF) : UniqueType(sF), position(), scale(1), rotation(), relativePosition(), relativeScale(1), relativeRotation(), relativeParentRotation() {}
    ~Transform3D() override {}
  };
}