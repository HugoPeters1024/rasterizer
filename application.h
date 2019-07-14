#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "mesh.h"
#include "linmath.h"
#include "camera.h"


class Application
{
  private:
    Mesh* person;
    Mesh* stack;
    Camera* camera;
    float fov;
  public:
    void init();
    void loop(int w, int h, Keyboard* keyboard);
    bool shouldClose();
};

void Application::init()
{
  person = new Mesh("male.obj");
  stack = new Mesh("stack.obj");
  stack->rotation[0] = 3.141592f / 2.0f;
  stack->position[1] = 10;
  camera = new Camera(1.25f);
  camera->pos[1] = 10.5f;
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  float ratio = w / (float)h;
  camera->update(keyboard);

  mat4x4 m_camera;
  camera->getMatrix(ratio, m_camera);

  person->update(keyboard);

  // Todo: pass camera pos to shader
  person->draw(m_camera);
  stack->draw(m_camera);

  stack->position[2] -= 0.005f;
  stack->rotation[2] += 0.01f;
}

bool Application::shouldClose() { return false; }

