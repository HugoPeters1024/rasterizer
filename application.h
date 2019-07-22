#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "utils.h"
#include "mesh.h"
#include "linmath.h"
#include "camera.h"


class Application
{
  private:
    Shader* shader;
    Mesh* person;
    Mesh* stack;
    Mesh* floor;
    Camera* camera;
    float fov;
  public:
    void init();
    void loop(int w, int h, Keyboard* keyboard);
    bool shouldClose();
};

void Application::init()
{
  shader = new Shader();
  person = new Mesh("male.obj");
  stack = new Mesh("stack.obj");
  floor = new Mesh("floor.obj");
  floor->scale = 10;
  stack->rotation[0] = 3.141592f / 2.0f;
  stack->position[1] = 10;
  person->rotation[1] = PI;
  camera = new Camera(1.25f);
  camera->pos[1] = 10.5f;
  camera->pos[2] = -10.5f;
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);

  person->update(keyboard);

  // Todo: pass camera pos to shader
  person->draw(shader, camera);
  stack->draw(shader, camera);
  floor->draw(shader, camera);

  stack->position[2] -= 0.005f;
  stack->rotation[2] += 0.01f;
  person->anchor[1] = -10;
  person->rotation[0] += 0.02f;
}

bool Application::shouldClose() { return false; }
