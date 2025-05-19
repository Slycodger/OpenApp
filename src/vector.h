#pragma once
#include <iostream>

namespace openApp {
  struct Vector2 {
    float x;
    float y;


    Vector2() : x(0), y(0) {}
    Vector2(float t) : x(t), y(t) {}
    Vector2(float a, float b) : x(a), y(b) {}
  };

  struct IVector2 {
    int x;
    int y;


    IVector2() : x(0), y(0) {}
    IVector2(int t) : x(t), y(t) {}
    IVector2(int a, int b) : x(a), y(b) {}
  };

  struct UIVector2 {
    unsigned int x;
    unsigned int y;


    UIVector2() : x(0), y(0) {}
    UIVector2(unsigned int t) : x(t), y(t) {}
    UIVector2(unsigned int a, unsigned int b) : x(a), y(b) {}
  };



  struct Vector3 {
    float x;
    float y;
    float z;


    Vector3 operator - (const Vector3& vec) const {
      Vector3 t = *this;
      t.x -= vec.x;
      t.y -= vec.y;
      t.z -= vec.z;
      return t;
    }
    Vector3 operator + (const Vector3& vec) const {
      Vector3 t = *this;
      t.x += vec.x;
      t.y += vec.y;
      t.z += vec.z;
      return t;
    }
    Vector3 operator / (const Vector3& vec) const {
      Vector3 t = *this;
      t.x /= vec.x;
      t.y /= vec.y;
      t.z /= vec.z;
      return t;
    }
    Vector3 operator * (const Vector3& vec) const {
      Vector3 t = *this;
      t.x *= vec.x;
      t.y *= vec.y;
      t.z *= vec.z;
      return t;
    }

    Vector3& operator += (const Vector3& vec) {
      x += vec.x;
      y += vec.y;
      z += vec.z;
      return *this;
    }
    Vector3& operator += (const float val) {
      x += val;
      y += val;
      z += val;
      return *this;
    }
    Vector3& operator *= (const float val) {
      x *= val;
      y *= val;
      z *= val;
      return *this;
    }

    friend std::ostream& operator <<(std::ostream& stream, const Vector3& vec);

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float t) : x(t), y(t), z(t) {}
    Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
  };

  struct IVector3 {
    int x;
    int y;
    int z;


    IVector3() : x(0), y(0), z(0) {}
    IVector3(int t) : x(t), y(t), z(t) {}
    IVector3(int a, int b, int c) : x(a), y(b), z(c) {}
  };

  struct UIVector3 {
    unsigned int x;
    unsigned int y;
    unsigned int z;


    UIVector3() : x(0), y(0), z(0) {}
    UIVector3(unsigned int t) : x(t), y(t), z(t) {}
    UIVector3(unsigned int a, unsigned int b, unsigned int c) : x(a), y(b), z(c) {}
  };



  struct Vector4 {
    float x;
    float y;
    float z;
    float w;


    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float t) : x(t), y(t), z(t), w(t) {}
    Vector4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
  };

  struct IVector4 {
    int x;
    int y;
    int z;
    int w;


    IVector4() : x(0), y(0), z(0), w(0) {}
    IVector4(int t) : x(t), y(t), z(t), w(t) {}
    IVector4(int a, int b, int c, int d) : x(a), y(b), z(c), w(d) {}
  };

  struct UIVector4 {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;


    UIVector4() : x(0), y(0), z(0), w(0) {}
    UIVector4(unsigned int t) : x(t), y(t), z(t), w(t) {}
    UIVector4(unsigned int a, unsigned int b, unsigned int c, unsigned int d) : x(a), y(b), z(c), w(d) {}
  };
}