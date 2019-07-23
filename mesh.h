#include "linmath.h"

#include "shader.h"
#include "resources.h"
#include "linmath.h"
#include "obj_loader.h"
#include "keyboard.h"
#include "camera.h"

class Mesh
{
private:
  GLuint vao;
  GLuint vbo, nbo, uvo;
  unsigned int triangle_count;

  void getMvp(mat4x4 m) const;
public:
  vec3 position, rotation, anchor;
  GLuint tex;
  float scale;
  Mesh(const ResourceManager* RM, const char* model);

  void draw(const Shader* shader, const Camera* camera) const;
  void update(Keyboard* keyboard);
};

Mesh::Mesh(const ResourceManager* RM, const char* model)
{
  vec3_zero(position);
  vec3_zero(rotation);
  vec3_zero(anchor);

  scale = 1;
  logDebug("Initializing Mesh");

  tex = RM->getTexture("white");

  auto obj = cObj(model);
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> uvs;
  obj.renderBuffers(vertices, normals, uvs);
  triangle_count = vertices.size() / 3;


  // Generate objects on GPU
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &nbo);
  glGenBuffers(1, &uvo);

  glBindVertexArray(vao);

  // Fill vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

  // Fill normals
  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);
  
  // Fill uvs
  glBindBuffer(GL_ARRAY_BUFFER, uvo);
  glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(float), uvs.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  logDebug("Done initializing mesh");
}

void Mesh::draw(const Shader* shader, const Camera* camera) const
{
   mat4x4 mvp;
   getMvp(mvp);
   glBindVertexArray(vao);
   shader->bind(camera, mvp);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer(shader->vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glVertexAttribPointer(shader->vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, uvo);
   glVertexAttribPointer(shader->vUV, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glDrawArrays(GL_TRIANGLES, 0, triangle_count);
   glBindVertexArray(0);
}

void Mesh::update(Keyboard* keyboard)
{
}

void Mesh::getMvp(mat4x4 f) const
{
  mat4x4 i, rx, ry, rz, t, ta, ta_r, s;
  mat4x4_identity(i);

  mat4x4_translate(t, position[0], position[1], position[2]);
  mat4x4_translate(ta, anchor[0], anchor[1], anchor[2]);
  mat4x4_translate(ta_r, -anchor[0], -anchor[1], -anchor[2]);
  mat4x4_scale_aniso(s, i, scale, scale, scale);
  mat4x4_rotate_X(rx, i, rotation[0]);
  mat4x4_rotate_Y(ry, i, rotation[1]);
  mat4x4_rotate_Z(rz, i, rotation[2]);

  mat4x4_mul(f, s, i);  // scale
  mat4x4_mul(f, ta, f); // translate anchor
  mat4x4_mul(f, rx, f); // rotate
  mat4x4_mul(f, ry, f);
  mat4x4_mul(f, rz, f);
  mat4x4_mul(f, ta_r, f); // reverse translate anchor
  mat4x4_mul(f, t, f);  // translate
}

