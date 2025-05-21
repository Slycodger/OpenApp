#pragma once
#include "vector.h"

namespace openApp {
  struct Vertex2D {
    Vector2 pos;
    Vector2 uv;

    Vertex2D() : pos(), uv() {}
    Vertex2D(Vector2 p, Vector2 u) : pos(p), uv(u) {}
  };
}