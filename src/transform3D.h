#pragma once
#include "uniqueType.h"
#include "vector.h"
#include "openglBasics.h"
#include "constants.h"
#include "staticList.h"
#include <iostream>

namespace openApp {
  class Transform3D : public UniqueType {
    StaticList<Transform3D*> children;
    size_t childIndex;
    Transform3D* parent;

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


    virtual void transform3DUpdate() {}
    virtual void transform3DAddedToGlobals() {}


    void uniqueTypeUpdate() override {
      if (!parent)
        updateTransformTree();

      transform3DUpdate();
    }
    void uniqueTypeAddedToGlobals() override {
      transform3DAddedToGlobals();
    }


    void updateTransformTree() {
      if (children.size() <= 0)
        return;

      for (Transform3D** childP : children) {

        //Get variables set up
        Transform3D* child = *childP;

        Vector3 diffRotation = rotation - child->relativeParentRotation;
        Vector3 relative = child->relativePosition * scale / child->relativeScale;
        Vector3 swap;

        float cosx = cos(diffRotation.x * _degToRadF);
        float sinx = sin(diffRotation.x * _degToRadF);

        float cosy = cos(diffRotation.y * _degToRadF);
        float siny = sin(diffRotation.y * _degToRadF);

        float cosz = cos(diffRotation.z * _degToRadF);
        float sinz = sin(diffRotation.z * _degToRadF);


        //Scale first
        child->scale = child->relativeScale * scale;

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
        child->position.x = relative.x;
        child->position.y = relative.y * sinx + relative.y * cosx;
        child->position.z = relative.z * -cosx - relative.y * -sinx;


        //Move to relative position
        child->position += position;

        //And set rotation to relative rotation
        child->rotation = rotation + child->relativeRotation;
        child->updateTransformTree();
      }
    }


    void setParent(Transform3D* p) {
      p->addChild(this);
    }

    void removeParent() {
      if (!parent)
        return;
      parent->children.removeAt(childIndex);
      childIndex = -1;
      parent = nullptr;
      relativePosition = Vector3();
      relativeScale = Vector3(1);
      relativeRotation = Vector3();
    }

    void addChild(Transform3D* c) {
      if (c->parent)
        c->removeParent();
      c->childIndex = children.addItem(c);
      if (c->childIndex >= (size_t)-1)
        return;

      c->relativePosition = c->position - position;
      c->relativeParentRotation = rotation;
      c->relativeScale = c->scale / scale;
      c->relativeRotation = c->rotation - rotation;
    }

    void removeChild(size_t& index) {
      Transform3D* c = *children[index];
      if (!c)
        return;
      c->removeParent();
    }


    Transform3D() : UniqueType(), position(), scale(1), rotation(), relativePosition(), relativeScale(1), relativeRotation(), children(1), childIndex(-1), parent(0) {}
  };
}