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
  mat4x4 ortho, perspective, camera;
  mat4x4_ortho(ortho, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mat4x4_perspective(perspective, 90, 1, 1.0f, 100.0f);
  mat4x4_mul(camera, ortho, perspective);
  dump(camera);
  mesh->update();
  mesh->draw(ortho);
}

bool Application::shouldClose() { return false; }

