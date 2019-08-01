#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "utils.h"
#include "mesh.h"
#include "linmath.h"
#include "camera.h"
#include "resources.h"


class Application
{
  private:
    ResourceManager* RM;
    IMesh* person;
    IMesh* stack;
    DefaultMesh* floor;
    Camera* camera;
    float fov;
  public:
    void init();
    void loop(int w, int h, Keyboard* keyboard);
    bool shouldClose();
};

void Application::init()
{
  RM = new ResourceManager();
  RM->loadTexture("floor", "texture.jpg");
  RM->loadTexture("white", "white.png");
  person = new DefaultMesh(RM, "male.obj");
  stack = new DefaultMesh(RM, "stack.obj");
  floor = new DefaultMesh(RM, "floor.obj");
  floor->tex = RM->getTexture("floor");
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
  person->draw(camera);
  stack->draw(camera);
  floor->draw(camera);

  stack->position[2] -= 0.005f;
  stack->rotation[2] += 0.01f;
  person->anchor[1] = -10;
  person->rotation[1] += 0.02f;
}

bool Application::shouldClose() { return false; }
