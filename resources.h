#ifndef RESOURCES_H
#define RESOURCES_H
#include <vector>
#include <string>
#include <map>

#include "logger.h"
#include "shaders.h"
#include "mesh.h"
#include "light.h"

class ResourceManager {
  private:
    std::map<std::string, GLuint> textures;
    std::map<std::string, IMesh*> meshes;
    DefaultShader* defaultShader;
    NormalMappedShader* normalMappedShader;
    void loadTexture(const char* handle, const char* filename);
    void loadMesh(const char* handle, IMesh* mesh) { meshes[handle] = mesh; }
  public:
    LightSet lightset;
    ResourceManager();
    GLuint getTexture(const char* handle) const { return textures.at(handle); }
    DefaultShader* getDefaultShader() const { return defaultShader; }
    NormalMappedShader* getNormalMappedShader() const { return normalMappedShader; }
    IMesh* getMesh(const char* handle) const { return meshes.at(handle); }
};

ResourceManager::ResourceManager()
{
  lightset[0].color = Vector3(1,1,0);
  lightset[0].brightness = 100;
  lightset[0].position.y = 10;

  lightset[1].color = Vector3(0,1,1);
  lightset[1].brightness = 100;
  lightset[1].position.y = 10;
  defaultShader = new DefaultShader(&lightset);
  normalMappedShader = new NormalMappedShader(&lightset);
  loadTexture("floor", "textures/texture.jpg");
  loadTexture("white", "textures/white.png");
  loadTexture("wall", "textures/wall.jpg");
  loadTexture("wall_norm", "textures/wall_norm.jpg");
  IMesh* player = new DefaultMesh(defaultShader, getTexture("white"), "models/player.obj");
  IMesh* floor = new NormalMappedMesh(normalMappedShader, getTexture("wall"), getTexture("wall_norm"), "models/floor.obj");
  loadMesh("floor", floor);
  loadMesh("player", player);
}


void ResourceManager::loadTexture(const char* handle, const char* filename)
{
  int width, height, nrChannels;
  GLuint texture;

  // Load the image
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!data) {
    logError("Could not load texture: %s", filename);
    exit(5);
  } else {
    logInfo("loaded texture %s with size %ix%i", filename, width, height);
  }

  // Generate texture resource
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // free image data on host
  stbi_image_free(data);

  // add handler to the map 
  textures[handle] = texture;
}

#endif
