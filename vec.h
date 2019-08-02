#ifndef VEC_H
#define VEC_H

#include <cmath>
#include "linmath.h"

class Vector2 {
public:
  float x, y;
  Vector2() : x(0), y(0) {}
  Vector2(float a) : x(a), y(a) {}
  Vector2(float x, float y) : x(x), y(y) {}

  float normalize() { float l = length(); x/=l; y/=l; }
  float length() const { return sqrt(x*x + y*y); }
  Vector2 operator * (const Vector2 &o) const { return Vector2(x*o.x, y*o.y); } 
  Vector2 operator * (float s) const    { return Vector2(x*s, y*s);     }
  Vector2 operator / (const Vector2 &o) const { return Vector2(x/o.x, y/o.y); } 
  Vector2 operator + (const Vector2 &o) const { return Vector2(x+o.x, y+o.y); } 
  Vector2& operator += (const Vector2 &o) { x += o.x, y += o.y; return *this; } 
  Vector2 operator - (const Vector2 &o) const { return Vector2(x-o.x, y-o.y); } 
  Vector2 operator - () const { return Vector2(-x, -y); }
  void print() { printf("(%f, %f)\n", x, y); }
};


class Vector3 {
public:
  float x, y, z;
  Vector3() : x(0), y(0), z(0) {}
  Vector3(float a) : x(a), y(a), z(a) {}
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  float normalize() { float l = length(); x/=l; y/=l; z/=l; }
  float length() const { return sqrt(x*x + y*y + z*z); }
  Vector3 operator * (const Vector3 &o) const { return Vector3(x*o.x, y*o.y, z*o.z); } 
  Vector3 operator * (float s) const    { return Vector3(x*s, y*s, z*s);       }
  Vector3 operator / (const Vector3 &o) const { return Vector3(x/o.x, y/o.y, z/o.z); }
  Vector3 operator + (const Vector3 &o) const { return Vector3(x+o.x, y+o.y, z+o.z); } 
  Vector3& operator += (const Vector3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
  Vector3 operator - (const Vector3 &o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
  Vector3 operator - () const { return Vector3(-x, -y, -z); }
  void print() { printf("(%f, %f, %f)\n", x, y, z); }
};

class Matrix4 {
private:
  mat4x4 data;
public:
  Matrix4() {};
  Matrix4(mat4x4 data) { mat4x4_dup(this->data, data); }
  void unpack(mat4x4 f) const { mat4x4_dup(f, data); }
  float* operator[] (int column) { return data[column]; }
  Matrix4 operator * (const Matrix4 &o) {
    mat4x4 r;
    mat4x4_mul(r, data, o.data);
    return Matrix4(r);
  }
  static Matrix4 FromTranslation(float x, float y, float z) {
    mat4x4 r;
    mat4x4_translate(r, x, y, z);
    return Matrix4(r);
  }
  static Matrix4 FromTranslation(Vector3 v) { return FromTranslation(v.x, v.y, v.z); }
  static Matrix4 FromScale(float x, float y, float z) {
    mat4x4 r;
    mat4x4_identity(r);
    mat4x4_scale_aniso(r, r, x, y, z);
    return Matrix4(r);
  }
  static Matrix4 FromScale(Vector3 v) { return FromScale(v.x, v.y, v.z); }
  static Matrix4 FromAxisRotations(float xr, float yr, float zr) {
    mat4x4 rx, ry, rz, i;
    mat4x4_identity(i);
    mat4x4_rotate_X(rx, i, xr);
    mat4x4_rotate_Y(ry, i, yr);
    mat4x4_rotate_Z(rz, i, zr);
    Matrix4 Rx = Matrix4(rx);
    Matrix4 Ry = Matrix4(ry);
    Matrix4 Rz = Matrix4(rz);
    return Rz * Ry * Rx;
  }
  static Matrix4 FromAxisRotations(Vector3 v) { return FromAxisRotations(v.x, v.y, v.z); }
  void print() const {
    for(int i=0; i<4; i++) {
      printf("%f, %f, %f, %f\n", data[0][i], data[1][i], data[2][i], data[3][i]);
    }
    printf("---------\n");
  }
};

#endif
