#pragma once
#include "transform2D.h"
#include "mesh2D.h"
#include "camera2D.h"
#include "program.h"
#include "material.h"

namespace openApp {
  class Visual2D : public Transform2D {
    size_t visual2DIndex;

  protected:
    virtual void visual2DUpdate() {}
    virtual void visual2DAddedToGlobals() {}
    virtual void visual2DRemovedFromGlobals() {}
    virtual void visual2DSetParent(UniqueType* ptr) {}
    virtual void visual2DAddedChild(UniqueType* ptr) {}
    virtual void visual2DRemovingChild(size_t index) {}

    void visual2DCopyTo(UniqueType* ptr) {}
    void transform2DCopyTo(UniqueType* ptr) override {
      Visual2D* vPtr = dynamic_cast<Visual2D*>(ptr);
      if (!vPtr)
        return;

      vPtr->srcMaterial = srcMaterial;
      vPtr->material = material;
      vPtr->mesh = mesh;
      vPtr->stencilLayers = stencilLayers;

      visual2DCopyTo(ptr);
    }

    void transform2DUpdate() override {

      visual2DUpdate();
    }

    void transform2DAddedToGlobals() override {
      if (visual2DIndex >= (size_t)-1)
        Visual2D::addGlobalVisual2DTree(this);

      visual2DAddedToGlobals();
    }
    void transform2DRemovedFromGlobals() override {
      Visual2D::removeGlobalVisual2D(this);

      visual2DRemovedFromGlobals();
    }
    void transform2DSetParent(UniqueType* ptr) override {

      visual2DSetParent(ptr);
    }
    void transform2DAddedChild(UniqueType* ptr) override {

      visual2DAddedChild(ptr);
    }
    void transform2DRemovingChild(size_t index) override {

      visual2DRemovingChild(index);
    }

  public:
    Material srcMaterial;
    Material* material;
    Mesh2D* mesh;
    int zIndex;
    unsigned char stencilLayers;


    UniqueType* create() override {
      return new Visual2D();
    }

    Visual2D() : Transform2D(), mesh(0), zIndex(0), stencilLayers(), visual2DIndex(-1), srcMaterial(), material(0) {}
    Visual2D(Mesh2D* mesh, int ZIndex, unsigned char StencilLayer) : Transform2D(), zIndex(ZIndex), mesh(mesh), stencilLayers(StencilLayer), visual2DIndex(-1), srcMaterial(), material(0) {}
    Visual2D(Mesh2D* mesh, int ZIndex, unsigned char StencilLayer, bool sF) : Transform2D(sF), zIndex(ZIndex), mesh(mesh), stencilLayers(StencilLayer), visual2DIndex(-1), srcMaterial(), material(0) {}
    ~Visual2D() override {
      if (selfContained) {
        if (mesh)
          delete(mesh);
        if (material)
          delete(material);
      }
    }



    //Static stuff
    //--------------------------------------------------
    static StaticList<Visual2D*> globalVisual2DInstances;
    static size_t globalVisual2DCount;



    //--------------------------------------------------
    static void addGlobalVisual2DTree(UniqueType* ptr) {
      for (UniqueType** c : ptr->children) {
        if (!c || !*c)
          continue;
        addGlobalVisual2DTree(*c);
      }


      UniqueType::addGlobalUniqueTypeTree(ptr);
      Visual2D* visual = dynamic_cast<Visual2D*>(ptr);
      if (!visual)
        return;
      if (visual->visual2DIndex < (size_t)-1)
        return;

      visual->visual2DIndex = globalVisual2DCount;
      globalVisual2DCount++;
      globalVisual2DInstances.addItem(visual);
    }



    //--------------------------------------------------
    static bool removeGlobalVisual2D(Visual2D* visual) {
      size_t i = visual->visual2DIndex;
      if (i >= (size_t)-1)
        return false;

      visual->visual2DIndex = -1;
      globalVisual2DCount--;
      globalVisual2DInstances.removeAt(i);

      return true;
    }



    //--------------------------------------------------
    static void end() {
      globalVisual2DInstances.clear();
    }



    //--------------------------------------------------
    static void drawVisual2D(Visual2D* visual) {
      if (!visual || !visual->mesh)
        return;

      glUseProgram(program::getShader2D());
      size_t camCount = Camera2D::getGlobalCamera2DCount();
      unsigned int VAO = program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, program::getShaderMaterialUBO());

      Shader::setMat4("transform", visual->getTransformMatrix());
      Shader::setInt("zIndex", visual->zIndex - 255);

      if (visual->material)
        visual->material->applyToShader();
      else
        visual->srcMaterial.applyToShader();

      glBindVertexBuffer(0, visual->mesh->getVBO(), 0, sizeof(Vertex2D));
      size_t vertexCount = visual->mesh->getVertexCount();


      glStencilFunc(GL_NOTEQUAL, 0, visual->stencilLayers);

      for (auto cam : Camera2D::globalCamera2DInstances) {
        (*cam)->active();
        Shader::setMat4("projection", (*cam)->getProjection());
        Shader::setMat4("view", (*cam)->getTransformMatrix());

        glDrawArrays(GL_TRIANGLES, 0, vertexCount * sizeof(Vertex2D));
      }

      glBindVertexArray(0);
    }




    //--------------------------------------------------
    static void drawVisual2DInstances() {
      Shader::setCurrentShader(program::getShader2D());
      StaticList<Camera2D*>& cams = Camera2D::globalCamera2DInstances;
      size_t camCount = Camera2D::getGlobalCamera2DCount();
      unsigned int VAO = program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, program::getShaderMaterialUBO());


      for (Visual2D** vP : Visual2D::globalVisual2DInstances) {
        Visual2D* visual = *vP;
        if (!visual->mesh)
          continue;
        Shader::setInt("zIndex", visual->zIndex - 255);
        Shader::setMat4("transform", visual->getTransformMatrix());

        if (visual->material)
          visual->material->applyToShader();
        else
          visual->srcMaterial.applyToShader();

        glBindVertexBuffer(1, visual->mesh->getVBO(), 0, sizeof(Vertex2D));
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