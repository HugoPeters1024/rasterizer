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
    std::vector<IGameObject*> meshes;
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
  gameInit(RM);

  camera = new Camera(1.25f);
  camera->pos[1] = 10.5f;
  camera->pos[2] = -10.5f;

  IMeshObject* floor = new Floor();
  floor->scale = Vector3(15); 

  IMeshObject* player = new Player();
  player->rotation.y = PI;

  meshes.push_back(floor);
  meshes.push_back(player);
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);

  for(IGameObject *mesh : meshes)
    mesh->draw(camera);
}

bool Application::shouldClose() { return false; }
