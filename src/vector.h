#pragma once
#include <iostream>

namespace openApp {
  struct Vector2 {
    float x;
    float y;

    Vector2 operator - (const Vector2& vec) const {
      Vector2 t = *this;
      t.x -= vec.x;
      t.y -= vec.y;
      return t;
    }
    Vector2 operator + (const Vector2& vec) const {
      Vector2 t = *this;
      t.x += vec.x;
      t.y += vec.y;
      return t;
    }
    Vector2 operator / (const Vector2& vec) const {
      Vector2 t = *this;
      t.x /= vec.x;
      t.y /= vec.y;
      return t;
    }
    Vector2 operator * (const Vector2& vec) const {
      Vector2 t = *this;
      t.x *= vec.x;
      t.y *= vec.y;
      return t;
    }

    Vector2& operator += (const Vector2& vec) {
      x += vec.x;
      y += vec.y;
      return *this;
    }
    Vector2& operator += (const float val) {
      x += val;
      y += val;
      return *this;
    }
    Vector2& operator *= (const float val) {
      x *= val;
      y *= val;
      return *this;
    }

    Vector2() : x(0), y(0) {}
    Vector2(float t) : x(t), y(t) {}
    Vector2(float a, float b) : x(a), y(b) {}
  };

  struct IVector2 {
    int x;
    int y;


    IVector2 operator - (const IVector2& vec) const {
      IVector2 t = *this;
      t.x -= vec.x;
      t.y -= vec.y;
      return t;
    }
    IVector2 operator + (const IVector2& vec) const {
      IVector2 t = *this;
      t.x += vec.x;
      t.y += vec.y;
      return t;
    }
    IVector2 operator / (const IVector2& vec) const {
      IVector2 t = *this;
      t.x /= vec.x;
      t.y /= vec.y;
      return t;
    }
    IVector2 operator * (const IVector2& vec) const {
      IVector2 t = *this;
      t.x *= vec.x;
      t.y *= vec.y;
      return t;
    }
    IVector2 operator * (const float& vec) const {
      IVector2 t = *this;
      t.x *= vec;
      t.y *= vec;
      return t;
    }

    IVector2& operator += (const IVector2& vec) {
      x += vec.x;
      y += vec.y;
      return *this;
    }

    IVector2& operator /= (const IVector2& vec) {
      x /= vec.x;
      y /= vec.y;
      return *this;
    }
    IVector2& operator += (const float val) {
      x += val;
      y += val;
      return *this;
    }
    IVector2& operator *= (const float val) {
      x *= val;
      y *= val;
      return *this;
    }

    IVector2() : x(0), y(0) {}
    IVector2(int t) : x(t), y(t) {}
    IVector2(int a, int b) : x(a), y(b) {}
  };

  struct UIVector2 {
    unsigned int x;
    unsigned int y;


    UIVector2 operator - (const UIVector2& vec) const {
      UIVector2 t = *this;
      t.x -= vec.x;
      t.y -= vec.y;
      return t;
    }
    UIVector2 operator + (const UIVector2& vec) const {
      UIVector2 t = *this;
      t.x += vec.x;
      t.y += vec.y;
      return t;
    }
    UIVector2 operator / (const UIVector2& vec) const {
      UIVector2 t = *this;
      t.x /= vec.x;
      t.y /= vec.y;
      return t;
    }
    UIVector2 operator * (const UIVector2& vec) const {
      UIVector2 t = *this;
      t.x *= vec.x;
      t.y *= vec.y;
      return t;
    }
    UIVector2 operator * (const float& vec) const {
      UIVector2 t = *this;
      t.x *= vec;
      t.y *= vec;
      return t;
    }

    UIVector2& operator += (const UIVector2& vec) {
      x += vec.x;
      y += vec.y;
      return *this;
    }
    UIVector2& operator += (const float val) {
      x += val;
      y += val;
      return *this;
    }
    UIVector2& operator *= (const float val) {
      x *= val;
      y *= val;
      return *this;
    }

    bool operator == (const UIVector2& v) const {
      return this->x == v.x && this->y == v.y;
    }
    bool operator != (const UIVector2& v) {
      return !(*this == v);
    }


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


    UIVector3 operator - (const UIVector3& vec) const {
      UIVector3 t = *this;
      t.x -= vec.x;
      t.y -= vec.y;
      t.z -= vec.z;
      return t;
    }
    UIVector3 operator + (const UIVector3& vec) const {
      UIVector3 t = *this;
      t.x += vec.x;
      t.y += vec.y;
      t.z += vec.z;
      return t;
    }
    UIVector3 operator / (const UIVector3& vec) const {
      UIVector3 t = *this;
      t.x /= vec.x;
      t.y /= vec.y;
      t.z /= vec.z;
      return t;
    }
    UIVector3 operator * (const UIVector3& vec) const {
      UIVector3 t = *this;
      t.x *= vec.x;
      t.y *= vec.y;
      t.z *= vec.z;
      return t;
    }

    UIVector3& operator += (const UIVector3& vec) {
      x += vec.x;
      y += vec.y;
      z += vec.z;
      return *this;
    }
    UIVector3& operator += (const float val) {
      x += val;
      y += val;
      z += val;
      return *this;
    }
    UIVector3& operator *= (const float val) {
      x *= val;
      y *= val;
      z *= val;
      return *this;
    }


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