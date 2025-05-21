#include "visual2D.h"
using namespace openApp;

StaticList<Visual2D*> Visual2D::globalVisual2DInstances = StaticList<Visual2D*>(32);
size_t Visual2D::globalVisual2DCount = 0;