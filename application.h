#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "mesh.h"
#include "linmath.h"
#include "camera.h"


class Application
{
  private:
    Mesh* mesh;
    Camera* camera;
    float fov;
  public:
    void init();
    void loop(int w, int h, Keyboard* keyboard);
    bool shouldClose();
};

void Application::init()
{
  mesh = new Mesh();
  camera = new Camera(1.25f);
  camera->pos[1] = 10.5f;
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  float ratio = w / (float)h;
  camera->update(keyboard);


  mat4x4 newCam;
  camera->getMatrix(ratio, newCam);

  mesh->update(keyboard);
  mesh->draw(newCam);
  mesh->position[0] += 6;
  mesh->draw(newCam);
  mesh->position[0] -= 6;
}

bool Application::shouldClose() { return false; }

