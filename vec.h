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
class BoundingBox {
public:
  Vector3 pos;
  Vector3 d;
  bool intersects(const BoundingBox &o) const {
    if (pos.x + d.x/2 <= o.pos.x - o.d.x/2 || pos.x - d.x/2 >= o.pos.x + o.d.x/2) return false;
    if (pos.y + d.y/2 <= o.pos.y - o.d.y/2 || pos.y - d.y/2 >= o.pos.y + o.d.y/2) return false;
    if (pos.z + d.z/2 <= o.pos.z - o.d.z/2 || pos.z - d.z/2 >= o.pos.z + o.d.z/2) return false;
    return true;
  }
  BoundingBox transformed(Matrix4 m) const {
    BoundingBox box;
    box.pos = (m * Vector4(pos, 1)).xyz();
    box.d = (m * Vector4(d, 0)).xyz();
  }
};

class AABB {
public:
  Vector3 lb, rt;
  bool intersects(const Ray &ray, float* t) const {
    *t = 0;

    Vector3 dirfrac;
    Vector3 ndir = ray.dir;
    dirfrac.x = 1.0f / ndir.x;
    dirfrac.y = 1.0f / ndir.y;
    dirfrac.z = 1.0f / ndir.z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // ray.origin is origin of ray
    float t1 = (lb.x - ray.origin.x)*dirfrac.x;
    float t2 = (rt.x - ray.origin.x)*dirfrac.x;
    float t3 = (lb.y - ray.origin.y)*dirfrac.y;
    float t4 = (rt.y - ray.origin.y)*dirfrac.y;
    float t5 = (lb.z - ray.origin.z)*dirfrac.z;
    float t6 = (rt.z - ray.origin.z)*dirfrac.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        *t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        *t = tmax;
        return false;
    }

    *t = tmin;

    return true;
  }
  bool intersects(const AABB &o) const {
    Ray ray1 = Ray((lb + rt) / 2, Vector3(rt.x - lb.x, 0, 0));
    Ray ray2 = Ray((lb + rt) / 2, Vector3(0, rt.y - lb.y, 0));
    Ray ray3 = Ray((lb + rt) / 2, Vector3(0, 0, rt.z - lb.z));
    float t;
    bool r1 = o.intersects(ray1, &t);
    printf("ray1 to (%f, %f, %f), getting t=%f, so isect? = %s\n", ray1.dir.x, ray1.dir.y, ray1.dir.z, t, r1 ? "yes" : "no");
    bool r2 = o.intersects(ray2, &t);
    printf("ray2 to (%f, %f, %f), getting t=%f, so isect? = %s\n", ray2.dir.x, ray2.dir.y, ray2.dir.z, t, r2 ? "yes" : "no");
    bool r3 = o.intersects(ray3, &t);
    printf("ray3 to (%f, %f, %f), getting t=%f, so isect? = %s\n", ray3.dir.x, ray3.dir.y, ray3.dir.z, t, r3 ? "yes" : "no");
    return r1 || r2 || r3;
  }
};

struct Line {
  Vector3 a, b;
  Line() {}
  Line(Vector3 a, Vector3 b) : a(a), b(b) {}
  bool parallel_overlap(const Line &o) {
      // assumes the lines are parallel
      return on_line(o.a) || on_line(o.b);
  }
  bool on_line(const Vector3 &c) {
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

class OBB {
public:
  std::array<Vector3, 8> points;
  Matrix4 m;
  Line project(const Vector3 &ax) const {
    Line line;
    bool first = true;
    for(const Vector3 &v : getPoints()) {
      Vector3 pv = ax * Vector3::dot(v, ax);
      if (first) { line.a = pv; line.b = pv; first = false; } else
      line.consume(pv);
    }
    return line;
  }

public:
  OBB(Vector3 pos, Vector3 dimensions) : m(Matrix4::Identity()) {

    Vector3 ux = Vector3(1, 0, 0);
    Vector3 uy = Vector3(0, 1, 0);
    Vector3 uz = Vector3(0, 0, 1);

    points = std::array<Vector3, 8>();
    points[0] = pos + dimensions.x * ux - dimensions.y * uy + dimensions.z * uz;
    points[1] = pos - dimensions.x * ux - dimensions.y * uy + dimensions.z * uz;
    points[2] = pos - dimensions.x * ux - dimensions.y * uy - dimensions.z * uz;
    points[3] = pos + dimensions.x * ux - dimensions.y * uy - dimensions.z * uz;

    points[4] = pos + dimensions.x * ux + dimensions.y * uy + dimensions.z * uz;
    points[5] = pos - dimensions.x * ux + dimensions.y * uy + dimensions.z * uz;
    points[6] = pos - dimensions.x * ux + dimensions.y * uy - dimensions.z * uz;
    points[7] = pos + dimensions.x * ux + dimensions.y * uy - dimensions.z * uz;
  }
  std::array<Vector3, 8> getPoints() const {
    std::array<Vector3, 8> ret;
    for(int i=0; i<8; i++) {
      ret[i] = (m * Vector4(points[i], 1)).xyz();
    }
    return ret;
  };
  std::array<Vector3, 3> getNormals() const {
    std::array<Vector3, 3> ret;
    auto vs = getPoints();
    ret[0] = (vs[1] - vs[0]).normalize();
    ret[1] = (vs[3] - vs[0]).normalize();
    ret[2] = (vs[4] - vs[0]).normalize();
    return ret;
  }
  bool intersects(const OBB &o) const {
     auto nleft  = o.getNormals();
     auto nright = getNormals();
     Vector3 axis[6];
     axis[0] = nleft[0];
     axis[1] = nleft[1];
     axis[2] = nleft[2];
     axis[3] = nright[0];
     axis[4] = nright[1];
     axis[5] = nright[2];
     for(const Vector3 &ax : axis) {
       Line p1 = project(ax);
       Line p2 = o.project(ax);
       if (!p1.parallel_overlap(p2)) return false;
     }
     return true;
  }

  void update(const Matrix4& mvp) {
    m = mvp;
  }
};


#endif
