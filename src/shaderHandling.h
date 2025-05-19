#pragma once
#include <string>
#include "openglBasics.h"
#include "vector.h"


namespace openApp {
  class Shader;
  struct ShaderPair;

  namespace shader {
    unsigned int getShader(std::string loc);
    unsigned int createShader(std::string name, ShaderPair* pairs, size_t shaderCount);
  }

  class Shader {
    unsigned int internal;

    static unsigned int currentShader;

  public:

    static void setBool(std::string name, bool value) {
      glUniform1i(glGetUniformLocation(currentShader, name.c_str()), value);
    }
    static void setUInt(std::string name, unsigned int value) {
      glUniform1ui(glGetUniformLocation(currentShader, name.c_str()), value);
    }
    static void setInt(std::string name, int value) {
      glUniform1i(glGetUniformLocation(currentShader, name.c_str()), value);
    }
    static void setFloat(std::string name, float value) {
      glUniform1f(glGetUniformLocation(currentShader, name.c_str()), value);
    }
    static void setVector2(std::string name, float x, float y) {
      glUniform2f(glGetUniformLocation(currentShader, name.c_str()), x, y);
    }
    static void setVector2(std::string name, Vector2 vec) {
      glUniform2f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y);
    }
    static void setVector3(std::string name, float x, float y, float z) {
      glUniform3f(glGetUniformLocation(currentShader, name.c_str()), x, y, z);
    }
    static void setVector3(std::string name, Vector3 vec) {
      glUniform3f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y, vec.z);
    }
    static void setVector4(std::string name, float x, float y, float z, float w) {
      glUniform4f(glGetUniformLocation(currentShader, name.c_str()), x, y, z, w);
    }
    static void setVector4(std::string name, Vector4 vec) {
      glUniform4f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    static void setMat4(std::string name, glm::mat4 Mat) {
      glUniformMatrix4fv(glGetUniformLocation(currentShader, name.c_str()), 1, false, glm::value_ptr(Mat));
    }
    static void setMat4(std::string name, const float* Mat) {
      glUniformMatrix4fv(glGetUniformLocation(currentShader, name.c_str()), 1, false, Mat);
    }


    static void setBool(const char* name, bool value) {
      glUniform1i(glGetUniformLocation(currentShader, name), value);
    }
    static void setUInt(const char* name, unsigned int value) {
      glUniform1ui(glGetUniformLocation(currentShader, name), value);
    }
    static void setInt(const char* name, int value) {
      glUniform1i(glGetUniformLocation(currentShader, name), value);
    }
    static void setFloat(const char* name, float value) {
      glUniform1f(glGetUniformLocation(currentShader, name), value);
    }
    static void setVector2(const char* name, float x, float y) {
      glUniform2f(glGetUniformLocation(currentShader, name), x, y);
    }
    static void setVector2(const char* name, Vector2 vec) {
      glUniform2f(glGetUniformLocation(currentShader, name), vec.x, vec.y);
    }
    static void setVector3(const char* name, float x, float y, float z) {
      glUniform3f(glGetUniformLocation(currentShader, name), x, y, z);
    }
    static void setVector3(const char* name, Vector3 vec) {
      glUniform3f(glGetUniformLocation(currentShader, name), vec.x, vec.y, vec.z);
    }
    static void setVector4(const char* name, float x, float y, float z, float w) {
      glUniform4f(glGetUniformLocation(currentShader, name), x, y, z, w);
    }
    static void setVector4(const char* name, Vector4 vec) {
      glUniform4f(glGetUniformLocation(currentShader, name), vec.x, vec.y, vec.z, vec.w);
    }
    static void setMat4(const char* name, glm::mat4 Mat) {
      glUniformMatrix4fv(glGetUniformLocation(currentShader, name), 1, false, glm::value_ptr(Mat));
    }
    static void setMat4(const char* name, const float* Mat) {
      glUniformMatrix4fv(glGetUniformLocation(currentShader, name), 1, false, Mat);
    }


    static unsigned int getCurrentShader() {
      return currentShader;
    }

    void active() {
      glUseProgram(internal);
      currentShader = internal;
    }
    
    Shader() {
      internal = 0;
    }
    Shader(unsigned int s) { internal = s; }

    operator int() {
      return internal;
    }
  };




  struct ShaderPair {
    const char** shaders;
    unsigned int shaderCount;
    unsigned int type;

    ShaderPair(const char** s, unsigned int sC, unsigned int t) : shaders(s), shaderCount(sC), type(t) {}
  };
}