#pragma once
#include "uniqueType.h"
#include "vector.h"
#include "openglBasics.h"
#include "constants.h"
#include "staticList.h"
#include <iostream>

namespace openApp {
  class Transform2D : public UniqueType {
  protected:
    virtual void transform2DCopyTo(UniqueType* ptr) {}
    virtual void transform2DUpdate() {}
    virtual void transform2DAddedToGlobals() {}
    virtual void transform2DRemovedFromGlobals() {}
    virtual void transform2DSetParent(UniqueType* ptr) {}
    virtual void transform2DAddedChild(UniqueType* ptr) {}
    virtual void transform2DRemovingChild(size_t index) {}

    void uniqueTypeSetParent(UniqueType* ptr) override {
      Transform2D* tPtr = dynamic_cast<Transform2D*>(ptr);
      if (!tPtr) {
        relativePosition = Vector2();
        relativeRotation = 0;
        relativeScale = Vector2(1);
        relativeParentRotation = 0;
        return;
      }

      relativePosition = position - tPtr->position;
      relativeParentRotation = tPtr->rotation;
      relativeScale = scale / tPtr->scale;
      relativeRotation = rotation - tPtr->rotation;

      transform2DSetParent(ptr);
    }

    void uniqueTypeAddedChild(UniqueType* ptr) override {

      transform2DAddedChild(ptr);
    }
    void uniqueTypeRemovingChild(size_t index) override {

      transform2DRemovingChild(index);
    }

    void uniqueTypeUpdate() override {
      if (!parent)
        updateTransformTree();

      transform2DUpdate();
    }
    void uniqueTypeAddedToGlobals() override {

       transform2DAddedToGlobals();
    }
    void uniqueTypeRemovedFromGlobals() override {

      transform2DRemovedFromGlobals();
    }


    void copyTo(UniqueType* ptr) override {
      Transform2D* tPtr = dynamic_cast<Transform2D*>(ptr);
      if (!tPtr)
        return;

      tPtr->position = position;
      tPtr->rotation = rotation;
      tPtr->scale = scale;
      tPtr->rotation = rotation;

      transform2DCopyTo(ptr);
    }

  public:

    Vector2 position;
    Vector2 scale;
    float rotation;
    Vector2 relativePosition;
    Vector2 relativeScale;
    float relativeRotation;
    float relativeParentRotation;

    virtual glm::mat4 getTransformMatrix() {
      glm::mat4 mat(1);
      mat = glm::translate(mat, glm::vec3(position.x, position.y, 0));
      mat = glm::rotate(mat, rotation * _degToRadF, glm::vec3(0, 0, 1));
      mat = glm::scale(mat, glm::vec3(scale.x, scale.y, 1));
      return mat;
    }



    UniqueType* create() override {
      return new Transform2D();
    }

    void updateTransform() {
      if (!parent)
        return;
      Transform2D* parent = dynamic_cast<Transform2D*>(this->parent);
      if (!parent)
        return;

      //Get variables set up
      float diffRotation = parent->rotation - relativeParentRotation;
      Vector2 relative = relativePosition * parent->scale / relativeScale;
      Vector2 swap;

      float c = cos(diffRotation * _degToRadF);
      float s = sin(diffRotation * _degToRadF);

      //Scale first
      scale = relativeScale * parent->scale;

      //Then rotated position
      position.x = relative.x * c - relative.y * s;
      position.y = relative.x * s + relative.y * c;


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
        Transform2D* tPtr = dynamic_cast<Transform2D*>(*childP);
        if (!tPtr)
          continue;

        tPtr->updateTransform();
        tPtr->updateTransformTree();
      }
    }


    Transform2D() : UniqueType(), position(), scale(1), rotation(), relativePosition(), relativeScale(1), relativeRotation(), relativeParentRotation() {}
    Transform2D(bool sF) : UniqueType(sF), position(), scale(1), rotation(), relativePosition(), relativeScale(1), relativeRotation(), relativeParentRotation() {}
    ~Transform2D() override {}
  };
}