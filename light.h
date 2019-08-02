#ifndef LIGHT_H
#define LIGHT_H

#include "vec.h"

struct Light {
  Vector3 position, color;
  float brightness;

  Light() : brightness(1) {}

  void sendToShader(GLint uPos, GLint uCol) const {
    glUniform3f(uPos, position.x, position.y, position.z);
    glUniform3f(uCol, color.x * brightness, color.y * brightness, color.z * brightness);
  }
};

struct LightSet {
  std::array<Light, 10> set;
  Light& operator [](int index) { return set[index]; }
  void sendToShader(std::array<GLint, 10> uPos, std::array<GLint, 10> uCol) const {
    for(int i=0; i<10; i++) {
      set[i].sendToShader(uPos[i], uCol[i]);
    }
  }
};

#endif
