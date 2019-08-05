#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <array>
#include "linmath.h"

class Vector2 {
public:
  float x, y;
  Vector2() : x(0), y(0) {}
  Vector2(float a) : x(a), y(a) {}
  Vector2(float x, float y) : x(x), y(y) {}

  Vector2& normalize() { float l = length(); x/=l; y/=l; return *this; }
  float length() const { return sqrt(x*x + y*y); }
  Vector2 operator * (const Vector2 &o) const { return Vector2(x*o.x, y*o.y); } 
  Vector2 operator * (float s) const    { return Vector2(x*s, y*s);     }
  Vector2 operator / (const Vector2 &o) const { return Vector2(x/o.x, y/o.y); } 
  Vector2 operator + (const Vector2 &o) const { return Vector2(x+o.x, y+o.y); } 
  Vector2& operator += (const Vector2 &o) { x += o.x; y += o.y; return *this; } 
  Vector2 operator - (const Vector2 &o) const { return Vector2(x-o.x, y-o.y); } 
  Vector2& operator -= (const Vector2 &o) { x -= o.x; y -= o.y; return *this; }
  Vector2 operator - () const { return Vector2(-x, -y); }
  void print() { printf("(%f, %f)\n", x, y); }
};


class Vector3 {
public:
  float x, y, z;
  Vector3() : x(0), y(0), z(0) {}
  Vector3(float a) : x(a), y(a), z(a) {}
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  Vector3& normalize() { float l = length(); x/=l; y/=l; z/=l; return *this; }
  Vector3 normalized() const { float l = length(); return Vector3(x/l, y/l, z/l); }
  float length() const { return sqrt(x*x + y*y + z*z); }
  static Vector3 cross(const Vector3 &a, const Vector3 &b) {
    vec3 v;
    vec3 va = { a.x, a.y, a.z };
    vec3 vb = { b.x, b.y, b.z };
    vec3_mul_cross(v, va, vb);
    return Vector3(v[0], v[1], v[2]);
  }
  static float dot(const Vector3 &a, const Vector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
  float sq_length() const { return dot(*this, *this); }
  Vector3 operator * (const Vector3 &o) const { return Vector3(x*o.x, y*o.y, z*o.z); } 
  Vector3 operator * (float s) const    { return Vector3(x*s, y*s, z*s);       }
  Vector3 operator / (const Vector3 &o) const { return Vector3(x/o.x, y/o.y, z/o.z); }
  Vector3 operator + (const Vector3 &o) const { return Vector3(x+o.x, y+o.y, z+o.z); } 
  Vector3& operator += (const Vector3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
  Vector3 operator - (const Vector3 &o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
  Vector3& operator -= (const Vector3 &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
  Vector3 operator - () const { return Vector3(-x, -y, -z); }
  void print() const { printf("(%f, %f, %f)\n", x, y, z); }
};

//Commutative mapping
Vector3 operator * (float s, const Vector3 &o) { return o * s; }

class Vector4 {
public:
  float x, y, z, w;
  Vector4() : x(0), y(0), z(0), w(0) {};
  Vector4(float a) : x(a), y(a), z(a), w(a) {}
  Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Vector4(Vector3 a, float w) : Vector4(a.x, a.y, a.z, w) {}

  Vector3 xyz() const { return Vector3(x, y, z); }
};

class Matrix4 {
private:
  mat4x4 data;
public:
  Matrix4() {}
  Matrix4(const mat4x4 data) { mat4x4_dup(this->data, data); }
  Matrix4(const Matrix4 &m) : Matrix4(m.data) {}
  void unpack(mat4x4 f) const { mat4x4_dup(f, data); }
  float* operator[] (int column) { return data[column]; }
  Matrix4 inverted() const { mat4x4 r; mat4x4_invert(r, data); return Matrix4(r); }
  Vector4 operator* (const Vector4 &o) const {
    vec4 r;
    vec4 i = { o.x, o.y, o.z, o.w };
    mat4x4_mul_vec4(r, data, i);
    return Vector4(r[0], r[1], r[2], r[3]);
  }
  Matrix4 operator * (const Matrix4 &o) const {
    mat4x4 r;
    mat4x4_mul(r, data, o.data);
    return Matrix4(r);
  }
  static Matrix4 Identity() { mat4x4 r; mat4x4_identity(r); return Matrix4(r); } 
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
    return Rx * Ry * Rz;
  }
  static Matrix4 FromAxisRotations(Vector3 v) { return FromAxisRotations(v.x, v.y, v.z); }
  static Matrix4 FromPerspective(float fov, float ratio, float znear, float zfar) {
    mat4x4 r;
    mat4x4_perspective(r, fov, ratio, znear, zfar);
    return Matrix4(r);
  }
  void print() const {
    for(int i=0; i<4; i++) {
      printf("%f, %f, %f, %f\n", data[0][i], data[1][i], data[2][i], data[3][i]);
    }
    printf("---------\n");
  }
};

class Ray {
public:
  Vector3 origin, dir;
  float length;

  Ray() {}
  Ray(Vector3 origin, Vector3 dir) : origin(origin), dir(dir) {}
};

class Plane {
public:
  Vector3 pos, normal, tangent, bitangent;
  bool intersects(const Ray &ray) const {
    // for any point p on the Plane, dot(p - pos, normal) = 0 holds
    // so we solve the t at which the ray intersects by using the parametric form as p.
    float denom = -Vector3::dot(ray.dir, normal); 
    if (denom > 1e-6) {   // near parallel is consired no real intersection
        float t = Vector3::dot(ray.origin - pos, normal) / denom;

        Vector3 point = ray.origin + ray.dir * t;
        Vector3 toPoint = point - pos;
        float ot = Vector3::dot(toPoint, tangent.normalized());
        float ob = Vector3::dot(toPoint, bitangent.normalized());

        // Half the distance to both side, but the number is squared, hence divide by 4
        float tl2 = Vector3::dot(tangent, tangent) / 4;
        float bl2 = Vector3::dot(bitangent, bitangent) / 4;

        if (ot * ot > tl2 || ot * ot < -tl2) return false;
        if (ob * ob > bl2 || ob * ob < -bl2) return false;

        if (t <= ray.length) return true;
    }
    return false;
  }
}; 

struct Line {
  Vector3 a, b;
  Line() {}
  Line(Vector3 a, Vector3 b) : a(a), b(b) {}
  bool parallel_overlap(const Line &o) const {
      // assumes the lines are parallel
      return on_line(o.a) || on_line(o.b) || o.on_line(a) || o.on_line(b);
  }
  bool on_line(const Vector3 &c) const {
    float AC = (a - c).length();
    float CB = (c - b).length();
    float AB = (a - b).length();
    // tolerance
    return fabs(AC + CB - AB) < 1e-4; 
  }
  void consume(const Vector3 &n) {
    if ((b-n).sq_length() > (b-a).sq_length() || (a-n).sq_length() > (a-b).sq_length())
    { 
      if ((b-n).sq_length() > (a-n).sq_length())
        a = n; 
      else
        b = n;
    }
  }
};



#endif
