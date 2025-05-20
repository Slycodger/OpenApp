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

    Mesh* handleMesh(const aiMesh* loadedMesh) {
      if (!loadedMesh)
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
      return mesh;
    }

    Visual3D* handleNode(const aiScene* scene, const aiNode* node, Visual3D* parent) {
      if (!node)
        return nullptr;

      Visual3D* visual = new Visual3D(nullptr, 0xff, true);
      if (node->mNumMeshes)
        visual->mesh = handleMesh(scene->mMeshes[node->mMeshes[0]]);

      if (parent)
        visual->setParent(parent);
      
      aiVector3t<ai_real> pos;
      aiVector3t<ai_real> rot;
      aiVector3t<ai_real> scale;
      node->mTransformation.Decompose(scale, rot, pos);
      if (parent) {
        visual->relativePosition = Vector3(pos.x, pos.y, pos.z);
        visual->relativeRotation = Vector3(rot.x * _radToDegF, rot.y * _radToDegF, rot.z * _radToDegF);
        visual->relativeScale = Vector3(scale.x, scale.y, scale.z);
        visual->relativeParentRotation = parent->rotation;
        visual->updateTransform();
      }
      else {
        visual->position = Vector3(pos.x, pos.y, pos.z);
        visual->rotation = Vector3(rot.x * _radToDegF, rot.y * _radToDegF, rot.z * _radToDegF);
        visual->scale = Vector3(scale.x, scale.y, scale.z);
      }

      for (unsigned int i = 0; i < node->mNumChildren; i++) {
        Visual3D* child = handleNode(scene, node->mChildren[i], visual);
      }

      return visual;
    }

    Visual3D* handleScene(const aiScene* scene) {
      if (!scene->mRootNode)
        return nullptr;

      Visual3D* visual = handleNode(scene, scene->mRootNode, nullptr);

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
