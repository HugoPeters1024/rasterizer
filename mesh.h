#ifndef MESH_H
#define MESH_H
#include "linmath.h"
#include "shaders.h"
#include "linmath.h"
#include "obj_loader.h"
#include "keyboard.h"
#include "camera.h"

class IMesh {
  public: 
    virtual void draw(const Camera* camera, Matrix4 m, float texSize=1) const = 0;
};


class DefaultMesh : public IMesh
{
private:
  DefaultShader* shader;
  GLuint vao;
  GLuint vbo, nbo, uvo;
  unsigned int triangle_count;

public:
  GLuint tex;
  DefaultMesh(DefaultShader* shader, GLuint tex, const char* model);
  void draw(const Camera* camera, Matrix4 m, float texSize=1) const override;
};

DefaultMesh::DefaultMesh(DefaultShader* shader, GLuint tex, const char* model) {
  logDebug("Initializing Mesh");

  this->tex = tex;
  this->shader = shader;

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

  // Assumes vao is bound
  shader->prepare(vbo, nbo, uvo);

  glBindVertexArray(0);

  logDebug("Done initializing mesh");
}

void DefaultMesh::draw(const Camera* camera, Matrix4 m, float texSize) const
{
   glBindVertexArray(vao);
   shader->bind(camera, m, tex, texSize);
   glDrawArrays(GL_TRIANGLES, 0, triangle_count);
   glBindVertexArray(0);
}

class NormalMappedMesh : public IMesh
{
private:
  NormalMappedShader* shader;
  GLuint vao;
  GLuint vbo, nbo, uvo, tbo, btbo;
  unsigned int triangle_count;

public:
  GLuint tex, n_tex;
  NormalMappedMesh(NormalMappedShader* shader, GLuint tex, GLuint n_tex, const char* model);
  void draw(const Camera* camera, Matrix4 m, float texSize=1) const override;
};

NormalMappedMesh::NormalMappedMesh(NormalMappedShader* shader, GLuint tex, GLuint n_tex, const char* model)
{
  logDebug("Initializing Mesh");

  this->tex = tex;
  this->n_tex = n_tex;
  this->shader = shader;

  auto obj = cObj(model);
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<float> tangents;
  std::vector<float> bitangents;
  obj.renderBuffersTangents(vertices, normals, uvs, tangents, bitangents);
  triangle_count = vertices.size() / 3;


  // Generate objects on GPU
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &nbo);
  glGenBuffers(1, &uvo);
  glGenBuffers(1, &tbo);
  glGenBuffers(1, &btbo);

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

  // Fill tangents
  glBindBuffer(GL_ARRAY_BUFFER, tbo);
  glBufferData(GL_ARRAY_BUFFER, tangents.size()*sizeof(float), tangents.data(), GL_STATIC_DRAW);

  // Fill bitangents
  glBindBuffer(GL_ARRAY_BUFFER, btbo);
  glBufferData(GL_ARRAY_BUFFER, bitangents.size()*sizeof(float), bitangents.data(), GL_STATIC_DRAW);

  // Assumes vao is bound
  shader->prepare(vbo, nbo, uvo, tbo, btbo);

  glBindVertexArray(0);

  logDebug("Done initializing mesh");
}

void NormalMappedMesh::draw(const Camera* camera, Matrix4 m, float texSize) const
{
   glBindVertexArray(vao);
   shader->bind(camera, m, tex, n_tex, texSize);
   glDrawArrays(GL_TRIANGLES, 0, triangle_count);
   glBindVertexArray(0);
}

#endif
