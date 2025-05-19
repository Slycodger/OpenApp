#pragma once
#include "vector.h"

namespace openApp {
  struct Vertex3D {
    Vector3 pos;
    Vector2 uv;

    Vertex3D() : pos(), uv() {}
    Vertex3D(Vector3 p, Vector2 u) : pos(p), uv(u) {}
  };
}