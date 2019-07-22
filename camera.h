#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "linmath.h"

class Camera
{
private:
   float fov;
   mat4x4 matrix;

   void calcMatrix(float ratio);

public:
   vec3 pos, viewDir;
   Camera(float fov);
   void update(float ratio, Keyboard* keyboard);
   void getMatrix(mat4x4 &t) const { mat4x4_dup(t, matrix); }
};


Camera::Camera(float fov)
{
  vec3_zero(pos);
  vec3_set(viewDir, 0, 0, 1);

  this->fov = fov;
}

void Camera::update(float ratio, Keyboard* keyboard)
{
  float speed = 0.3f;
  float rot_speed = 0.02f;
  vec3 move_dir = { viewDir[0], 0, viewDir[2] };
  vec3_norm(move_dir, move_dir);
  vec3 unitY = { 0, 1, 0};

  // Sideways move vector
  vec3 move_tan = { -move_dir[2], move_dir[1], move_dir[0] }; //tangent to move_dir 
  vec3_scale(move_tan, move_tan, speed);

  // Forwards move vector
  vec3 move_par; //parallel to to move_dir
  vec3_scale(move_par, move_dir, speed);

  // Up/down move vector
  vec3 move_vert;
  vec3_scale(move_vert, unitY, speed);

  // Tangent to view vector in xz plane
  vec3 view_tan;
  vec3_mul_cross(view_tan, move_dir, unitY);
  vec3_norm(view_tan, view_tan);
  vec3_scale(view_tan, view_tan, rot_speed);

  // Tangent to view vector and view tangent
  vec3 view_bitan;
  vec3_mul_cross(view_bitan, viewDir, view_tan);
  vec3_norm(view_bitan, view_bitan);
  vec3_scale(view_bitan, view_bitan, rot_speed);

  if (keyboard->isDown(MOVE_FORWARD))   vec3_add(pos, pos, move_par);
  if (keyboard->isDown(MOVE_BACKWARD))  vec3_sub(pos, pos, move_par); 
  if (keyboard->isDown(MOVE_LEFT))      vec3_sub(pos, pos, move_tan); 
  if (keyboard->isDown(MOVE_RIGHT))     vec3_add(pos, pos, move_tan); 
  if (keyboard->isDown(MOVE_UP))        vec3_add(pos, pos, move_vert); 
  if (keyboard->isDown(MOVE_DOWN))      vec3_sub(pos, pos, move_vert); 

  // Change to adding tangent or bi-tangent and then normalizing
  if (keyboard->isDown(LOOK_UP)) {
    float ny = viewDir[1] + view_bitan[1];
    if (ny < 0.99 && ny > -0.99) vec3_add(viewDir, viewDir, view_bitan);
  }
  if (keyboard->isDown(LOOK_DOWN)) {
    float ny = viewDir[1] - view_bitan[1];
    if (ny < 0.99 && ny > -0.99)
      vec3_sub(viewDir, viewDir, view_bitan); 
  }
  if (keyboard->isDown(LOOK_LEFT))      vec3_sub(viewDir, viewDir, view_tan); 
  if (keyboard->isDown(LOOK_RIGHT))     vec3_add(viewDir, viewDir, view_tan); 
  vec3_norm(viewDir, viewDir);

  calcMatrix(ratio);
}

void Camera::calcMatrix(float screenRatio)
{
  mat4x4 i, rx, ry, rz, t, p, f;
  mat4x4_identity(i);
  mat4x4_identity(t);

  mat4x4_perspective(p, fov, screenRatio, 0.1f, 1000.0f);
  mat4x4_translate(t, -pos[0], -pos[1], -pos[2]);

  float theta, phi;
  vec3 xz = { viewDir[0], 0, viewDir[2] };
  vec3_norm(xz, xz);

  float vx = viewDir[0];
  float vy = viewDir[1];
  float vz = viewDir[2];

  theta = -atan2(xz[0], -xz[2]);
  if (theta < 0)
    theta = theta + 2 * 3.1415926f;

  float l_vxz = sqrtf(vx * vx + vz * vz); //length of xz
  phi = atan2(viewDir[1], l_vxz);

  mat4x4_rotate_Y(ry, i, theta);
  mat4x4_rotate_X(rx, i, phi);

  mat4x4_mul(f, t, i);   // 1. translate
  mat4x4_mul(f, ry, f);  // 2. rotate
  mat4x4_mul(f, rx, f);  
  mat4x4_mul(f, p, f);   // 3. apply perspective
  mat4x4_dup(matrix, f);
}



#endif
