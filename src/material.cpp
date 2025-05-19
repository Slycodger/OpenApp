#include "material.h"
using namespace openApp;

std::map<std::string, Material*> Material::globalMaterialInstances = std::map<std::string, Material*>();