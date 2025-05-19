#include "openglBasics.h"
#include "shaderHandling.h"
#include <map>
#include <string>
#include <iostream>
#include "exceptionHandling.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>

using namespace openApp;

std::map<std::string, unsigned int> globalShaders = std::map<std::string, unsigned int>();

unsigned int Shader::currentShader = 0;

namespace openApp {
  namespace shader {
    unsigned int getShader(std::string loc) {
      if (!globalShaders.contains(loc))
        return 0u;

      return globalShaders[loc];
    }

    unsigned int createShader(std::string name, ShaderPair* pairs, size_t shaderCount) {
      if (globalShaders.contains(name))
        return globalShaders[name];

      if (!shaderCount || !pairs)
        return 0;
      for (size_t i = 0; i < shaderCount; i++)
        if (!pairs[i].shaderCount || !pairs[i].shaders)
          return 0;
        else for (size_t j = 0; j < pairs[i].shaderCount; j++)
          if (!pairs[i].shaders[j])
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

      globalShaders.insert({ name, shaderProgram });
      return shaderProgram;
    }
  }
}