#pragma once
#include "transform3D.h"
#include "mesh3D.h"
#include "camera3D.h"
#include "program.h"
#include "material.h"

namespace openApp {
  class Visual3D : public Transform3D {
    size_t visual3DIndex;

  protected:
    virtual void visual3DUpdate() {}
    virtual void visual3DAddedToGlobals() {}
    virtual void visual3DRemovedFromGlobals() {}
    virtual void visual3DSetParent(UniqueType* ptr) {}
    virtual void visual3DAddedChild(UniqueType* ptr) {}
    virtual void visual3DRemovingChild(size_t index) {}

    void visual3DCopyTo(UniqueType* ptr) {}
    void transform3DCopyTo(UniqueType* ptr) override {
      Visual3D* vPtr = dynamic_cast<Visual3D*>(ptr);
      if (!vPtr)
        return;

      vPtr->srcMaterial = srcMaterial;
      vPtr->material = material;
      vPtr->mesh = mesh;
      vPtr->stencilLayers = stencilLayers;

      visual3DCopyTo(ptr);
    }

    void transform3DUpdate() override {

      visual3DUpdate();
    }

    void transform3DAddedToGlobals() override {
      if (visual3DIndex >= (size_t)-1)
        Visual3D::addGlobalVisual3DTree(this);

      visual3DAddedToGlobals();
    }

    void transform3DRemovedFromGlobals() override {
      Visual3D::removeGlobalVisual3D(this);

      visual3DRemovedFromGlobals();
    }

    void transform3DSetParent(UniqueType* ptr) override {

      visual3DSetParent(ptr);
    }
    void transform3DAddedChild(UniqueType* ptr) override {

      visual3DAddedChild(ptr);
    }
    void transform3DRemovingChild(size_t index) override {

      visual3DRemovingChild(index);
    }

  public:
    Material srcMaterial;
    Material* material;
    Mesh3D* mesh;
    unsigned char stencilLayers;


    UniqueType* create() override {
      return new Visual3D();
    }

    Visual3D() : Transform3D(), mesh(0), stencilLayers(), visual3DIndex(-1), srcMaterial(), material(0) {}
    Visual3D(Mesh3D* mesh, unsigned char StencilLayer) : Transform3D(), mesh(mesh), stencilLayers(StencilLayer), visual3DIndex(-1), srcMaterial(), material(0) {}
    Visual3D(Mesh3D* mesh, unsigned char StencilLayer, bool sF) : Transform3D(sF), mesh(mesh), stencilLayers(StencilLayer), visual3DIndex(-1), srcMaterial(), material(0) {}
    ~Visual3D() override {
      if (selfContained) {
        if (mesh)
          delete(mesh);
        if (material)
          delete(material);
      }
    }



    //Static stuff
    //--------------------------------------------------
    static StaticList<Visual3D*> globalVisual3DInstances;
    static size_t globalVisual3DCount;



    //--------------------------------------------------
    static void addGlobalVisual3DTree(UniqueType* ptr) {
      for (UniqueType** c : ptr->children) {
        if (!c || !*c)
          continue;
        addGlobalVisual3DTree(*c);
      }


      UniqueType::addGlobalUniqueTypeTree(ptr);
      Visual3D* visual = dynamic_cast<Visual3D*>(ptr);
      if (!visual)
        return;
      if (visual->visual3DIndex < (size_t)-1)
        return;

      visual->visual3DIndex = globalVisual3DCount;
      globalVisual3DCount++;
      globalVisual3DInstances.addItem(visual);
    }



    //--------------------------------------------------
    static bool removeGlobalVisual3D(Visual3D* visual) {
      size_t i = visual->visual3DIndex;
      if (i >= (size_t)-1)
        return false;

      visual->visual3DIndex = -1;
      globalVisual3DCount--;
      globalVisual3DInstances.removeAt(i);
      return true;
    }



    //--------------------------------------------------
    static void end() {
      Visual3D::globalVisual3DInstances.clear();
    }



    //--------------------------------------------------
    static void drawVisual3D(Visual3D* visual) {
      if (!visual || !visual->mesh)
        return;

      glUseProgram(program::getShader3D());
      size_t camCount = Camera3D::getGlobalCamera3DCount();
      unsigned int VAO = program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, program::getShaderMaterialUBO());

      Shader::setMat4("transform", visual->getTransformMatrix());

      if (visual->material)
        visual->material->applyToShader();
      else
        visual->srcMaterial.applyToShader();

      glBindVertexBuffer(0, visual->mesh->getVBO(), 0, sizeof(Vertex3D));
      size_t vertexCount = visual->mesh->getVertexCount();


      glStencilFunc(GL_NOTEQUAL, 0, visual->stencilLayers);

      for (auto cam : Camera3D::globalCamera3DInstances) {
        (*cam)->active();
        Shader::setMat4("projection", (*cam)->getProjection());
        Shader::setMat4("view", (*cam)->getTransformMatrix());

        glDrawArrays(GL_TRIANGLES, 0, vertexCount * sizeof(Vertex3D));
      }

      glBindVertexArray(0);
    }




    //--------------------------------------------------
    static void drawVisual3DInstances() {
      Shader::setCurrentShader(program::getShader3D());
      StaticList<Camera3D*>& cams = Camera3D::globalCamera3DInstances;
      size_t camCount = Camera3D::getGlobalCamera3DCount();
      unsigned int VAO = program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, program::getShaderMaterialUBO());


      for (Visual3D** vP : Visual3D::globalVisual3DInstances) {
        Visual3D* visual = *vP;
        if (!visual->mesh)
          continue;
        Shader::setMat4("transform", visual->getTransformMatrix());

        if (visual->material)
          visual->material->applyToShader();
        else
          visual->srcMaterial.applyToShader();

        glBindVertexBuffer(0, visual->mesh->getVBO(), 0, sizeof(Vertex3D));
        size_t vertexCount = visual->mesh->getVertexCount();


        glStencilFunc(GL_NOTEQUAL, 0, visual->stencilLayers);

        for (auto cam : cams) {
          (*cam)->active();
          Shader::setMat4("projection", (*cam)->getProjection());
          Shader::setMat4("view", (*cam)->getTransformMatrix());

          glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
      }

      glBindVertexArray(0);
    }
  };
}