#ifndef VEC_H
#define VEC_H

#include <cmath>

class Vector2 {
public:
  float x, y;
  Vector2() { x=0; y=0; };
  Vector2(float a) { x=a; y=a; };
  Vector2(float x, float y) : x(x), y(y) {};

  float normalize() { float l = length(); x/=l; y/=l; };
  float length() const { return sqrt(x*x + y*y); };
  Vector2 operator * (const Vector2 &o) { return Vector2(x*o.x, y*o.y); }; 
  Vector2 operator / (const Vector2 &o) { return Vector2(x/o.x, y/o.y); }; 
  Vector2 operator + (const Vector2 &o) { return Vector2(x+o.x, y+o.y); }; 
  Vector2 operator - (const Vector2 &o) { return Vector2(x-o.x, y-o.y); }; 
};

class Vector3 {
public:
  float x, y, z;
  Vector3() { x=0; y=0; z=0; };
  Vector3(float a) { x=a; y=a; z=a; };
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

  float normalize() { float l = length(); x/=l; y/=l; z/=l; };
  float length() const { return sqrt(x*x + y*y + z*z); };
  Vector3 operator * (const Vector3 &o) { return Vector3(x*o.x, y*o.y, z*o.z); }; 
  Vector3 operator / (const Vector3 &o) { return Vector3(x/o.x, y/o.y, z/o.z); }; 
  Vector3 operator + (const Vector3 &o) { return Vector3(x+o.x, y+o.y, z+o.z); }; 
  Vector3 operator - (const Vector3 &o) { return Vector3(x-o.x, y-o.y, z-o.z); }; 
};

#endif
