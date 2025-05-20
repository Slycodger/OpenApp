#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "modelLoading.h"

using namespace openApp;
using namespace openApp::modelLoading;

std::map<std::string, Visual3D*> modelsLoaded = std::map<std::string, Visual3D*>();


namespace openApp {
  namespace modelLoading {
    Visual3D* handleScene(const aiScene* scene) {
      if (!scene->HasMeshes())
        return nullptr;
      aiMesh* loadedMesh = scene->mMeshes[0];
      if (!loadedMesh->HasPositions() || !loadedMesh->HasFaces())
        return nullptr;

      Vertex3D* sharedVerts = new Vertex3D[loadedMesh->mNumVertices];
      Vertex3D* verts = new Vertex3D[loadedMesh->mNumFaces * 3];

      for (unsigned int i = 0; i < loadedMesh->mNumVertices; i++)
        sharedVerts[i] = Vertex3D(((Vector3*)loadedMesh->mVertices)[i], Vector2());
      for (unsigned int i = 0; i < loadedMesh->mNumFaces; i++) {
        aiFace face = loadedMesh->mFaces[i];
        verts[(i * 3)] = sharedVerts[face.mIndices[0]];
        verts[(i * 3) + 1] = sharedVerts[face.mIndices[1]];
        verts[(i * 3) + 2] = sharedVerts[face.mIndices[2]];
      }
      delete[](sharedVerts);

      Mesh* mesh = new Mesh(verts, loadedMesh->mNumFaces * 3, true);
      Visual3D* visual = new Visual3D(mesh, 1, true);
      return visual;
    }



    Visual3D* loadModel(std::string path) {
      if (modelsLoaded.contains(path))
        return modelsLoaded[path];

      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_SortByPType);
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        modelsLoaded.insert({ path, nullptr });
        return nullptr;
      }
      Visual3D* visual = handleScene(scene);
      modelsLoaded.insert({ path, visual });
      return visual;
    }



    void end() {
      for (std::pair<std::string, Visual3D*> p : modelsLoaded) {
        if (p.second)
          delete(p.second);
      }
      modelsLoaded.clear();
    }
  }
}
