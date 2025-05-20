#include "shaderHandling.h"


using namespace openApp;

std::map<std::string, unsigned int> Shader::globalShaders = std::map<std::string, unsigned int>();

unsigned int Shader::currentShader = 0;