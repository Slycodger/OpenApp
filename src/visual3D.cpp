#define _VISUAL3D
#include "visual3D.h"
using namespace openApp;

Shader Visual3D::visual3DShader = 0;
unsigned int Visual3D::materialUBO = 0;

StaticList<Visual3D*> Visual3D::globalVisual3DInstances = StaticList<Visual3D*>(32);
size_t Visual3D::globalVisual3DCount = 0;