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
  RM = new ResourceManager();
  RM->loadTexture("floor", "texture.jpg");
  RM->loadTexture("white", "white.png");
  shader = new Shader();
  person = new Mesh(RM, "male.obj");
  stack = new Mesh(RM, "stack.obj");
  floor = new Mesh(RM, "floor.obj");
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
  person->draw(shader, camera);
  stack->draw(shader, camera);
  floor->draw(shader, camera);

  stack->position[2] -= 0.005f;
  stack->rotation[2] += 0.01f;
  person->anchor[1] = -10;
  person->rotation[1] += 0.02f;
}

bool Application::shouldClose() { return false; }
