#pragma once
#include "transform3D.h"
#include "mesh.h"
#include "camera3D.h"
#include "program.h"
#include "material.h"

namespace openApp {
  class Visual3D : public Transform3D {
    size_t index;
    bool selfContained;

    static Shader visual3DShader;
    static unsigned int materialUBO;

  public:
    Material srcMaterial;
    Material* material;
    Mesh* mesh;
    unsigned int stencilLayers;


    virtual void visual3D() {}

    void transform3DUpdate() override {

      visual3D();
    }

    Visual3D(Mesh* mesh, unsigned int StencilLayer) : Transform3D(), mesh(mesh), stencilLayers(StencilLayer), index(-1), srcMaterial(), material(0), selfContained(false) {}
    Visual3D(Mesh* mesh, unsigned int StencilLayer, bool sF) : Transform3D(), mesh(mesh), stencilLayers(StencilLayer), index(-1), srcMaterial(), material(0), selfContained(sF) {}
    ~Visual3D() {
      if (selfContained && mesh)
        delete(mesh);
    }



    //Static stuff
    //--------------------------------------------------
    static StaticList<Visual3D*> globalVisual3DInstances;
    static size_t globalVisual3DCount;



    //--------------------------------------------------
    static bool addGlobalVisual3D(Visual3D* visual) {
      if (visual->index < (size_t)-1)
        return false;
      visual->index = globalVisual3DCount;
      globalVisual3DCount++;
      globalVisual3DInstances.addItem(visual);
      UniqueType::addGlobalUniqueType(visual);
      return true;
    }



    //--------------------------------------------------
    static bool removeGlobalVisual3D(Visual3D* visual) {
      size_t i = visual->index;
      if (i >= (size_t)-1)
        return false;

      visual->index = -1;
      globalVisual3DCount--;
      globalVisual3DInstances.removeAt(i);
      return true;
    }



    //--------------------------------------------------
    static void start() {
      const char* vertShader[] = {
    "./shaders/basicShader.vert"
      };
      const char* fragShader[] = {
      "./shaders/basicShader.frag"
      };
      ShaderPair shaderPair[2] = { ShaderPair{vertShader, 1, GL_VERTEX_SHADER}, ShaderPair(fragShader, 1, GL_FRAGMENT_SHADER) };
      visual3DShader = Shader::createShader("visual3DShader", shaderPair, 2);

      glCreateBuffers(1, &materialUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(Material::ShaderMaterial), 0, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, 0, materialUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }



    //--------------------------------------------------
    static void end() {
      Visual3D::globalVisual3DInstances.clear();
    }



    //--------------------------------------------------
    static void drawVisual3D(Visual3D* visual) {
      if (!visual || !visual->mesh)
        return;

      visual3DShader.active();
      size_t camCount = Camera3D::getCameraCount();
      unsigned int VAO = Program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);

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
      visual3DShader.active();
      StaticList<Camera3D*>& cams = Camera3D::globalCamera3DInstances;
      size_t camCount = Camera3D::getCameraCount();
      unsigned int VAO = Program::getGlobalVAO();

      glBindVertexArray(VAO);
      glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);


      for (Visual3D** vP : Visual3D::globalVisual3DInstances) {
        Visual3D* visual = *vP;
        if (!visual->mesh)
          continue;
        visual3DShader.setMat4("transform", visual->getTransformMatrix());

        if (visual->material)
          visual->material->applyToShader();
        else
          visual->srcMaterial.applyToShader();

        glBindVertexBuffer(0, visual->mesh->getVBO(), 0, sizeof(Vertex3D));
        size_t vertexCount = visual->mesh->getVertexCount();


        glStencilFunc(GL_NOTEQUAL, 0, visual->stencilLayers);

        for (auto cam : cams) {
          (*cam)->active();
          visual3DShader.setMat4("projection", (*cam)->getProjection());
          visual3DShader.setMat4("view", (*cam)->getTransformMatrix());

          glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
      }

      glBindVertexArray(0);
    }
  };
}