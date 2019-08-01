#ifndef RESOURCES_H
#define RESOURCES_H
#include <vector>
#include <string>
#include <map>

#include "logger.h"

class ResourceManager {
  private:
    std::map<std::string, GLuint> textures;
    Shader* defaultShader;
  public:
    ResourceManager();
    void loadTexture(const char* handle, const char* filename);
    GLuint getTexture(const char* handle) const { return textures.at(handle); };
    Shader* getDefaultShader() const { return defaultShader; };
};

ResourceManager::ResourceManager()
{
  defaultShader = new Shader();
}


void ResourceManager::loadTexture(const char* handle, const char* filename)
{
  int width, height, nrChannels;
  GLuint texture;

  // Load the image
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!data) {
    logError("Could not load texture: %s", filename);
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
