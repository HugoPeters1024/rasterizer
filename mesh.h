#include "linmath.h"
#include "shaders.h"
#include "resources.h"
#include "linmath.h"
#include "obj_loader.h"
#include "keyboard.h"
#include "camera.h"
#include "abstract.h"


class DefaultMesh : public IMesh
{
private:
  DefaultShader* shader;
  GLuint vao;
  GLuint vbo, nbo, uvo;
  unsigned int triangle_count;

public:
  GLuint tex;
  DefaultMesh(const ResourceManager* RM, const char* model);
  void draw(const Camera* camera) const override;
  void update(Keyboard* keyboard) override;
};

DefaultMesh::DefaultMesh(const ResourceManager* RM, const char* model) {
  logDebug("Initializing Mesh");

  tex = RM->getTexture("white");
  shader = RM->getDefaultShader();

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

void DefaultMesh::draw(const Camera* camera) const
{
   mat4x4 mvp;
   getMvp(mvp);
   glBindVertexArray(vao);
   shader->bind(camera, mvp, tex);
   glDrawArrays(GL_TRIANGLES, 0, triangle_count);
   glBindVertexArray(0);
}

void DefaultMesh::update(Keyboard* keyboard)
{
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
  NormalMappedMesh(const ResourceManager* RM, const char* model);
  void draw(const Camera* camera) const override;
  void update(Keyboard* keyboard) override;
};

NormalMappedMesh::NormalMappedMesh(const ResourceManager* RM, const char* model)
{
  vec3_zero(position);
  vec3_zero(rotation);
  vec3_zero(anchor);

  scale = 1;
  logDebug("Initializing Mesh");

  tex = RM->getTexture("wall");
  n_tex = RM->getTexture("wall_norm");
  shader = RM->getNormalMappedShader();

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

void NormalMappedMesh::draw(const Camera* camera) const
{
   mat4x4 mvp;
   getMvp(mvp);
   glBindVertexArray(vao);
   shader->bind(camera, mvp, tex, n_tex);
   glDrawArrays(GL_TRIANGLES, 0, triangle_count);
   glBindVertexArray(0);
}

void NormalMappedMesh::update(Keyboard* keyboard)
{
}

