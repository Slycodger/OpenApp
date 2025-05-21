#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "modelLoading.h"

using namespace openApp;
using namespace openApp::modelLoading;

std::map<std::string, UniqueType*> modelsLoaded = std::map<std::string, UniqueType*>();


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

    Transform3D* handleNode(const aiScene* scene, const aiNode* node, Transform3D* parent) {
      if (!node)
        return nullptr;

      Transform3D* ret = nullptr;
      Visual3D* asV = nullptr;
      if (node->mNumMeshes) {
        asV = new Visual3D(nullptr, 0xff, true);
        ret = asV;
        asV->mesh = handleMesh(scene->mMeshes[node->mMeshes[0]]);
      } else
        ret = new Transform3D(true);




      if (parent)
        ret->setParent(parent);
      
      aiVector3t<ai_real> pos;
      aiVector3t<ai_real> rot;
      aiVector3t<ai_real> scale;
      node->mTransformation.Decompose(scale, rot, pos);
      if (parent) {
        ret->relativePosition = Vector3(pos.x, pos.y, pos.z);
        ret->relativeRotation = Vector3(rot.x * _radToDegF, rot.y * _radToDegF, rot.z * _radToDegF);
        ret->relativeScale = Vector3(scale.x, scale.y, scale.z);
        ret->relativeParentRotation = parent->rotation;
        ret->updateTransform();
      }
      else {
        ret->position = Vector3(pos.x, pos.y, pos.z);
        ret->rotation = Vector3(rot.x * _radToDegF, rot.y * _radToDegF, rot.z * _radToDegF);
        ret->scale = Vector3(scale.x, scale.y, scale.z);
      }

      for (unsigned int i = 0; i < node->mNumChildren; i++) {
        handleNode(scene, node->mChildren[i], ret);
      }

      return ret;
    }

    UniqueType* handleScene(const aiScene* scene) {
      if (!scene->mRootNode)
        return nullptr;

      UniqueType* visual = handleNode(scene, scene->mRootNode, nullptr);

      return visual;
    }



    UniqueType* loadModel(std::string path) {
      if (modelsLoaded.contains(path))
        return modelsLoaded[path];

      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_SortByPType);
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        modelsLoaded.insert({ path, nullptr });
        return nullptr;
      }
      UniqueType* visual = handleScene(scene);
      modelsLoaded.insert({ path, visual });
      return visual;
    }



    void end() {
      for (std::pair<std::string, UniqueType*> p : modelsLoaded) {
        if (p.second)
          delete(p.second);
      }
      modelsLoaded.clear();
    }
  }
}
