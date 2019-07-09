#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "mesh.h"
#include "linmath.h"


class Application
{
  private:
    Mesh* mesh;
    Mesh* mesh2;
    vec3 camPos;
  public:
    void init();
    void loop(int w, int h);
    bool shouldClose();
};

void Application::init()
{
  mesh = new Mesh();
  mesh2 = new Mesh();
  camPos[0] = 0;
  camPos[1] = 0;
  camPos[2] = 0;
}

void Application::loop(int w, int h)
{
  float ratio = w / (float)h;
  mat4x4 perspective, camera, view;
  mat4x4_perspective(perspective, 1.2f, ratio, 0.1f, 1000.0f);
  mat4x4_identity(camera);
  mat4x4_translate(camera, -camPos[0], -camPos[1], camPos[2]);
  mat4x4_mul(view, perspective, camera);

  mesh->update();
  mesh2->update();
  mesh2->update();

  mesh->draw(view);
  mesh2->draw(view);

  mesh2->position[0] += 0.01f;

  camPos[1] += 0.01f;
}

bool Application::shouldClose() { return false; }

