#include "camera3D.h"
using namespace openApp;

Camera3D* Camera3D::mainCamera3D = 0;
size_t Camera3D::globalCamera3DCount = 0;
StaticList<Camera3D*> Camera3D::globalCamera3DInstances = StaticList<Camera3D*>();