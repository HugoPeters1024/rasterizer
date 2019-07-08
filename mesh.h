#include <GLFW/glfw3.h>
#include "linmath.h"

#include "shader.h"
#include "linmath.h"
#include "obj_loader.h"

class Mesh
{
private:
  GLuint vao;
  GLuint vbo;
  Shader* shader;
  cObj* obj;

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
  logDebug("Initializing Mesh");

  obj = new cObj("male.obj");

  float* triangleData = (float*)malloc(9*sizeof(float));
  triangleData[0] = 0;
  triangleData[1] = -1;
  triangleData[2] = 0;

  triangleData[3] = 1;
  triangleData[4] = 1;
  triangleData[5] = 0;

  triangleData[6] = -1;
  triangleData[7] = 1;
  triangleData[8] = 0;

  std::vector<float> vertices;
  for(face f : obj->faces)
  {
    for(int i : f.vertex)
    {
      printf("%i\n", i);
      vertices.push_back(obj->vertices[i].v[0]);
      vertices.push_back(obj->vertices[i].v[1]);
      vertices.push_back(obj->vertices[i].v[2]);
    }
    printf("\n");
  }

  // Generate objects on GPU
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  // Fill vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

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
   glDrawArrays(GL_TRIANGLES, 0, obj->faces.size()*6);
   glBindVertexArray(0);
}

void Mesh::update()
{
  position[1] += 0.005f;
}

void Mesh::getMvp(mat4x4 m) const
{
  mat4x4_identity(m);
  mat4x4_translate(m, position[0], position[1], position[2]);
  mat4x4_scale(m, m, 0.002f);
//  mat4x4_rotate(m, m, 1, 0, 0, rotation[0]);
 // mat4x4_rotate(m, m, 0, 1, 0, rotation[1]);
  //mat4x4_rotate(m, m, 0, 0, 1, rotation[2]);
}

