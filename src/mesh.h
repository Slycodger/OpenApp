#pragma once
#include "vertex3D.h"
#include "exceptionHandling.h"
#include "openglBasics.h"
#include "material.h"
#include <map>

namespace openApp {
  class Mesh {
    unsigned int VBO;
    size_t triCount;
    size_t vertexCount;
    Vertex3D* vertices;

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

    Vertex3D operator [](unsigned int pos) {
      if (pos < 0 || pos >= vertexCount) {
        openApp::logError("OUT-OF-RANGE", "openApp::Mesh::[]", "pos", "position past allowed area");
        return {};
      }
      return vertices[pos];
    }

    Mesh() = delete;
    Mesh(Vertex3D* verts, size_t vertCount) : vertices(verts), vertexCount(vertCount), triCount(vertCount / 3) {
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex3D), verts, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }




    //Static stuff
    //--------------------------------------------------
    static std::map<std::string, Mesh*> globalMeshInstances;



    //--------------------------------------------------
    static bool addGlobalMesh(std::string name, Mesh* mesh) {
      if (globalMeshInstances.contains(name))
        return false;
      globalMeshInstances.insert({ name, mesh });
    }



    //--------------------------------------------------
    static Mesh* getGlobalMesh(std::string name) {
      if (!globalMeshInstances.contains(name))
        return 0;

      return globalMeshInstances[name];
    }



    //--------------------------------------------------
    static bool removeGlobalMesh(std::string name) {
      if (!globalMeshInstances.contains(name))
        return false;
      globalMeshInstances.erase(name);
      return true;
    }



    //--------------------------------------------------
    static void end() {
      Mesh::globalMeshInstances.clear();
    }
  };
}