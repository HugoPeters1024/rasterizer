#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "linmath.h"

struct Camera
{
   vec3 pos, viewDir;
   float fov;

   Camera(float fov);
   void update(Keyboard* keyboard);
   void getMatrix(float screenRatio, mat4x4 &f) const;
};

Camera::Camera(float fov)
{
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;

  viewDir[0] = 0;
  viewDir[1] = 0.5;
  viewDir[2] = 1;
  vec3_norm(viewDir, viewDir);

  this->fov = fov;
}

void Camera::update(Keyboard* keyboard)
{
  float speed = 0.3f;
  vec3 move_dir = { viewDir[0], 0, viewDir[2] };
  vec3_norm(move_dir, move_dir);
  vec3 up = { 0, 1, 0 };
  vec3 tan; //tangent to viewDir
  vec3_mul_cross(tan, up, move_dir);
  vec3_scale(tan, tan, speed);

  vec3 par; //par to viewDir
  vec3_scale(par, move_dir, speed);
  logDebug("%f, %f, %f", viewDir[0], par[1], par[2]);


  if (keyboard->isDown(MOVE_FORWARD))   vec3_add(pos, pos, par);
  if (keyboard->isDown(MOVE_BACKWARD))  vec3_sub(pos, pos, par); 
  if (keyboard->isDown(MOVE_LEFT))      vec3_sub(pos, pos, tan); 
  if (keyboard->isDown(MOVE_RIGHT))     vec3_add(pos, pos, tan); 

  // Change to adding tangent or bi-tangent and then normalizing
  if (keyboard->isDown(LOOK_UP))        viewDir[1] += 0.02f;
  if (keyboard->isDown(LOOK_DOWN))      viewDir[1] -= 0.02f;
  if (keyboard->isDown(LOOK_LEFT))      viewDir[0] -= 0.02f;
  if (keyboard->isDown(LOOK_RIGHT))     viewDir[0] += 0.02f;
  vec3_norm(viewDir, viewDir);
}

void Camera::getMatrix(float screenRatio, mat4x4 &f) const
{
  mat4x4 i, rx, ry, rz, t, p;
  mat4x4_identity(i);
  mat4x4_identity(t);

  mat4x4_perspective(p, fov, screenRatio, 0.1f, 1000.0f);
  mat4x4_translate(t, -pos[0], -pos[1], pos[2]);

  float theta = atan2(viewDir[1], viewDir[2]); 
  float phi   = atan2(viewDir[0], viewDir[2]);
  
  mat4x4_rotate_X(ry, i, -theta);
  mat4x4_rotate_Y(rx, i, -phi);

  mat4x4_mul(f, t, i);   // 1. translate
  mat4x4_mul(f, rx, f);  
  mat4x4_mul(f, ry, f);  // 2. rotate
  mat4x4_mul(f, p, f);   // 3. apply perspective
}



#endif
