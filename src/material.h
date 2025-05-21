#pragma once
#include "openglBasics.h"
#include "shaderHandling.h"
#include "texture.h"
#include <map>
#include <string>

namespace openApp {
  class Material {
  public:
    struct ShaderMaterial {
      Vector4 albedo;
      unsigned int usesAlbedoTexture;
      ShaderMaterial(Vector4 a, unsigned int u) : albedo(a), usesAlbedoTexture(u) {}
    };

    Vector4 albedo;
    Texture* albedoTexture;


    ShaderMaterial forShader() {
      return { albedo, albedoTexture ? 1ul : 0ul };
    }

    void applyToShader() {
      ShaderMaterial shaderMaterial = forShader();
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderMaterial), &shaderMaterial);
      if (albedoTexture) {
        glBindTexture(GL_TEXTURE_2D, albedoTexture->getInternal());
        glActiveTexture(GL_TEXTURE0);
        Shader::setInt("albedoTexture", 0);
      }
    }

    Material() {
      albedo = { 1, 1, 1, 1 };
      albedoTexture = 0;
    }




    //Static stuff
    //--------------------------------------------------
    static std::map<std::string, Material*> globalMaterialInstances;



    //--------------------------------------------------
    static bool addGlobalMaterial(std::string name, Material* mat) {
      return globalMaterialInstances.insert({ name, mat }).second;
    }



    //--------------------------------------------------
    static Material* getGlobalMaterial(std::string name) {
      if (!globalMaterialInstances.contains(name))
        return 0;
      return globalMaterialInstances[name];
    }



    //--------------------------------------------------
    static bool removeGlobalMaterial(std::string name) {
      if (!globalMaterialInstances.contains(name))
        return false;
      globalMaterialInstances.erase(name);
      return true;
    }



    //--------------------------------------------------
    static void end() {
      globalMaterialInstances.clear();
    }
  };
}