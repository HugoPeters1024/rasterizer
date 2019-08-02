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
    float time;
  public:
    void init();
    void loop(int w, int h, Keyboard* keyboard);
    bool shouldClose();
};

void Application::init()
{
  time=0;
  RM = new ResourceManager();
  gameInit(RM);

  camera = new Camera(1.25f);
  camera->pos.y = 10.5f;
  camera->pos.z = -10.5f;

  Floor* floor = new Floor();
  Floor* wall = new Floor();
  wall->rotation.x = -PI / 2;
  wall->position.z = 15;
  wall->position.y = 15;

  Player* player = new Player();
  player->rotation.y = PI;

  meshes.push_back(floor);
  meshes.push_back(wall);
  meshes.push_back(player);
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  time+=0.03f;
  RM->lightset[0].position.x = 11 * sin(time);
  RM->lightset[0].position.z = 11 * cos(time);
  RM->lightset[1].position.x = -11 * sin(time);
  RM->lightset[1].position.z = -11 * cos(time);
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);

  for(IGameObject *mesh : meshes)
    mesh->update(keyboard);
  for(IGameObject *mesh : meshes)
    mesh->draw(camera);
}

bool Application::shouldClose() { return false; }
