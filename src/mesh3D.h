#pragma once
#include "vertex3D.h"
#include "exceptionHandling.h"
#include "openglBasics.h"
#include "material.h"
#include <map>

namespace openApp {
  class Mesh3D {
    unsigned int VBO;
    size_t triCount;
    size_t vertexCount;
    Vertex3D* vertices;
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

    Vertex3D operator [](unsigned int pos) {
      if (pos < 0 || pos >= vertexCount) {
        openApp::logError("OUT-OF-RANGE", "openApp::Mesh::[]", "pos", "position past allowed area");
        return {};
      }
      return vertices[pos];
    }

    Mesh3D() = delete;
    Mesh3D(Vertex3D* verts, size_t vertCount) : vertices(verts), vertexCount(vertCount), triCount(vertCount / 3), selfContained(false) {
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex3D), verts, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    Mesh3D(Vertex3D* verts, size_t vertCount, bool sF) : vertices(verts), vertexCount(vertCount), triCount(vertCount / 3), selfContained(sF) {
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex3D), verts, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~Mesh3D() {
      if (selfContained && vertices)
        delete[](vertices);
    }




    //Static stuff
    //--------------------------------------------------
    static std::map<std::string, Mesh3D*> globalMesh3DInstances;



    //--------------------------------------------------
    static bool addGlobalMesh3D(std::string name, Mesh3D* mesh) {
      if (globalMesh3DInstances.contains(name))
        return false;
      globalMesh3DInstances.insert({ name, mesh });
      return true;
    }



    //--------------------------------------------------
    static Mesh3D* getGlobalMesh3D(std::string name) {
      if (!globalMesh3DInstances.contains(name))
        return 0;

      return globalMesh3DInstances[name];
    }



    //--------------------------------------------------
    static bool removeGlobalMesh3D(std::string name) {
      if (!globalMesh3DInstances.contains(name))
        return false;
      globalMesh3DInstances.erase(name);
      return true;
    }



    //--------------------------------------------------
    static void end() {
      Mesh3D::globalMesh3DInstances.clear();
    }
  };
}