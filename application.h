#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "mesh.h"
#include "linmath.h"


class Application
{
  private:
    Mesh* mesh;
  public:
    void init();
    void loop(int w, int h);
    bool shouldClose();
};

void Application::init()
{
  mesh = new Mesh();
}

void Application::loop(int w, int h)
{
  float ratio = w / (float)h;
  mat4x4 camera;
  mat4x4_ortho(camera, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mesh->update();
  mesh->draw(camera);
}

bool Application::shouldClose() { return false; }

