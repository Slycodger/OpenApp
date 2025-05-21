#include "visual3D.h"
using namespace openApp;

StaticList<Visual3D*> Visual3D::globalVisual3DInstances = StaticList<Visual3D*>(32);
size_t Visual3D::globalVisual3DCount = 0;