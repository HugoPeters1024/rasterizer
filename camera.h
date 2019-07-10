#ifndef CAMERA_H
#define CAMERA_H

#include "linmath.h"

struct Camera
{
   vec3 pos, rotation;
   float fov;

   Camera(float fov);
   void getMatrix(float screenRatio, mat4x4 &f) const;
};

Camera::Camera(float fov)
{
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;

  rotation[0] = 0;
  rotation[1] = 0;
  rotation[2] = 0;

  this->fov = fov;
}

void Camera::getMatrix(float screenRatio, mat4x4 &f) const
{
  mat4x4 i, rx, ry, rz, t, p;
  mat4x4_identity(i);
  mat4x4_identity(t);

  mat4x4_perspective(p, fov, screenRatio, 0.1f, 1000.0f);
  mat4x4_translate(t, -pos[0], -pos[1], pos[2]);
  mat4x4_rotate_X(rx, i, -rotation[0]);
  mat4x4_rotate_Y(ry, i, -rotation[1]);
  mat4x4_rotate_Z(rz, i, -rotation[2]);


  mat4x4_mul(f, t, i);   // 1. translate
  mat4x4_mul(f, rx, f);  
  mat4x4_mul(f, ry, f);
  mat4x4_mul(f, rz, f);  // 2. rotate
  mat4x4_mul(f, p, f);   // 3. apply perspective

}



#endif
