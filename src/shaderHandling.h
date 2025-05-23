#pragma once
#include <string>
#include "openglBasics.h"
#include "vector.h"
#include <map>
#include <string>
#include <iostream>
#include "exceptionHandling.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include "program.h"


namespace openApp {
  struct ShaderPair {
    const char** shaders;
    unsigned int shaderCount;
    unsigned int type;

    ShaderPair(const char** s, unsigned int sC, unsigned int t) : shaders(s), shaderCount(sC), type(t) {}
  };

  class Shader {
    unsigned int internal;

    static unsigned int currentShader;

  public:
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




    static std::map<std::string, unsigned int> globalShaders;


    static unsigned int getShader(std::string loc) {
      if (!globalShaders.contains(loc))
        return 0u;

      return globalShaders[loc];
    }

    static unsigned int createShader(std::string name, ShaderPair* pairs, size_t shaderCount) {
      if (name != "" && globalShaders.contains(name))
        return globalShaders[name];

      if (!shaderCount || !pairs)
        return 0;
      for (size_t i = 0; i < shaderCount; i++)
        if (!pairs[i].shaderCount || !pairs[i].shaders)
          return 0;
        else for (size_t j = 0; j < pairs[i].shaderCount; j++)
          if (!pairs[i].shaders[j] || !program::fileExists(pairs[i].shaders[j]))
            return 0;


      unsigned int shaderProgram = glCreateProgram();
      unsigned int* shaders = new unsigned int[shaderCount];
      for (size_t i = 0; i < shaderCount; i++)
        shaders[i] = 0;

      char err[255];
      int success = 0;

      for (size_t shaderIndex = 0; shaderIndex < shaderCount; shaderIndex++) {
        try {
          char** shaderStr = new char* [pairs[shaderIndex].shaderCount];

          for (size_t shaderPieceIndex = 0; shaderPieceIndex < pairs[shaderIndex].shaderCount; shaderPieceIndex++) {
            std::ifstream file(pairs[shaderIndex].shaders[shaderPieceIndex], std::ios::binary | std::ios::in | std::ios::ate);

            size_t fileSize = 0;
            fileSize = file.tellg();
            file.seekg(std::ios::beg);


            shaderStr[shaderPieceIndex] = new char[fileSize + 1];

            std::stringstream sstream;
            std::string str;
            sstream << file.rdbuf();
            str = sstream.str();
            std::strcpy(shaderStr[shaderPieceIndex], str.data());

            file.close();
          }

          shaders[shaderIndex] = glCreateShader(pairs[shaderIndex].type);
          glShaderSource(shaders[shaderIndex], pairs[shaderIndex].shaderCount, shaderStr, 0);
          glCompileShader(shaders[shaderIndex]);
          glGetShaderiv(shaders[shaderIndex], GL_COMPILE_STATUS, &success);
          if (!success) {
            glGetShaderInfoLog(shaders[shaderIndex], 255, 0, err);
            logError("SHADER COMPILING", "openApp::shader::getShader", "shaders[" + shaderIndex + ']', std::string("error compiling shader --- ") + err);
          }

          glAttachShader(shaderProgram, shaders[shaderIndex]);

          for (size_t j = 0; j < pairs[shaderIndex].shaderCount; j++)
            delete[](shaderStr[j]);
          delete[](shaderStr);
        }
        catch (std::exception e) {
          logError("FILE", "openApp::shader::getShader", "", std::string("shader file failed to open --- ") + e.what());
        }
      }

      glLinkProgram(shaderProgram);
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shaderProgram, 255, 0, err);
        logError("SHADER PROGRAM LINKING", "openApp::shader::getShader", "shaderProgram", std::string("error linking shader program --- ") + err);
      }

      for (size_t i = 0; i < shaderCount; i++)
        glDeleteShader(shaders[i]);
      delete[](shaders);

      if (name != "")
        globalShaders.insert({ name, shaderProgram });
      return shaderProgram;
    }

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
  };
}