#include <GLFW/glfw3.h>
#include "linmath.h"

#include "shader.h"
#include "linmath.h"
#include "obj_loader.h"

class Mesh
{
private:
  GLuint vao;
  GLuint vbo, nbo;
  Shader* shader;
  std::vector<float> vertices;
  std::vector<float> normals;

  void getMvp(mat4x4 m) const;
public:
  vec3 position, rotation;
  float scale;
  Mesh();
  void draw(const mat4x4 camera) const;
  void update();
};

Mesh::Mesh()
{
  position[0] = 0;
  position[1] = 0;
  position[2] = 1.2f;
  rotation[0] = 0;
  rotation[1] = 0;
  rotation[2] = 0;
  scale = 0.05f;
  logDebug("Initializing Mesh");

  auto obj = cObj("male.obj");
  obj.renderBuffers(vertices, normals);


  // Generate objects on GPU
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &nbo);

  glBindVertexArray(vao);

  // Fill vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

  // Fill vertices
  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);


  // Create shader
  shader = new Shader();

  // Mount buffers to shader fields
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(
      shader->vPos,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)(sizeof(float) * 0));

  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glVertexAttribPointer(
      shader->vNormal,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)(sizeof(float) * 0));

  glBindVertexArray(0);

  logDebug("Done initializing mesh");
}

void Mesh::draw(const mat4x4 camera) const
{
   mat4x4 mvp;
   getMvp(mvp);
   shader->bind();
   glBindVertexArray(vao);
   glUniformMatrix4fv(shader->uMvp, 1, GL_FALSE, (const GLfloat*)mvp);
   glUniformMatrix4fv(shader->uCamera, 1, GL_FALSE, (const GLfloat*)camera);
   glDrawArrays(GL_TRIANGLES, 0, vertices.size());
   glBindVertexArray(0);
}

void Mesh::update()
{
  rotation[1] += 0.05f;
  rotation[0] += 0.02f;
}

void Mesh::getMvp(mat4x4 f) const
{
  mat4x4 i, rx, ry, rz, t, s;
  mat4x4_identity(i);
  mat4x4_identity(rx);
  mat4x4_identity(ry);
  mat4x4_identity(rz);
  mat4x4_identity(t);
  mat4x4_identity(s);
  mat4x4_identity(f);
  mat4x4_translate(t, position[0], position[1], position[2]);
  mat4x4_scale_aniso(s, i, scale, scale, scale);
  mat4x4_rotate_X(rx, i, rotation[0]);
  mat4x4_rotate_Y(ry, i, rotation[1]);
  mat4x4_rotate_Z(rz, i, rotation[2]);
  mat4x4_mul(f, f, t);
  mat4x4_mul(f, s, f);
  mat4x4_mul(f, f, rx);
  mat4x4_mul(f, f, ry);
  mat4x4_mul(f, f, rz);

//  mat4x4_rotate(m, m, 1, 0, 0, rotation[0]);
 // mat4x4_rotate(m, m, 0, 1, 0, rotation[1]);
  //mat4x4_rotate(m, m, 0, 0, 1, rotation[2]);
}

