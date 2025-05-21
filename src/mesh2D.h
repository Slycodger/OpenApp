#pragma once
#include "vertex2D.h"
#include "exceptionHandling.h"
#include "openglBasics.h"
#include "material.h"
#include <map>

namespace openApp {
  class Mesh2D {
    unsigned int VBO;
    size_t triCount;
    size_t vertexCount;
    Vertex2D* vertices;
    bool selfContained;

  public:
    unsigned int getVBO() {
      return VBO;
    }

    size_t getTriCount() {
      return triCount;
    }

    size_t getVertexCount() {
      return vertexCount;
    }

    Vertex2D operator [](unsigned int pos) {
      if (pos < 0 || pos >= vertexCount) {
        openApp::logError("OUT-OF-RANGE", "openApp::Mesh::[]", "pos", "position past allowed area");
        return {};
      }
      return vertices[pos];
    }

    Mesh2D() = delete;
    Mesh2D(Vertex2D* verts, size_t vertCount) : vertices(verts), vertexCount(vertCount), triCount(vertCount / 3), selfContained(false) {
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex2D), verts, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    Mesh2D(Vertex2D* verts, size_t vertCount, bool sF) : vertices(verts), vertexCount(vertCount), triCount(vertCount / 3), selfContained(sF) {
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex2D), verts, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~Mesh2D() {
      if (selfContained && vertices)
        delete[](vertices);
    }




    //Static stuff
    //--------------------------------------------------
    static std::map<std::string, Mesh2D*> globalMesh2DInstances;



    //--------------------------------------------------
    static bool addGlobalMesh2D(std::string name, Mesh2D* mesh) {
      if (globalMesh2DInstances.contains(name))
        return false;
      globalMesh2DInstances.insert({name, mesh});
      return true;
    }



    //--------------------------------------------------
    static Mesh2D* getGlobalMesh2D(std::string name) {
      if (!globalMesh2DInstances.contains(name))
        return 0;

      return globalMesh2DInstances[name];
    }



    //--------------------------------------------------
    static bool removeGlobalMesh2D(std::string name) {
      if (!globalMesh2DInstances.contains(name))
        return false;
      globalMesh2DInstances.erase(name);
      return true;
    }



    //--------------------------------------------------
    static void end() {
      Mesh2D::globalMesh2DInstances.clear();
    }
  };
}