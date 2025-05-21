#include "camera2D.h"
using namespace openApp;

Camera2D* Camera2D::mainCamera3D = 0;
size_t Camera2D::globalCamera2DCount = 0;
StaticList<Camera2D*> Camera2D::globalCamera2DInstances = StaticList<Camera2D*>();