#include <stdio.h>
#include <fstream>

#include "obj_loader.h"

#include "vec.h"
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
    Floor* ramp;
    Floor* floor;
    Player* player;
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

  floor = new Floor();
  Floor* back = new Floor();
  back->rotation.x = -PI / 2;
  back->position.z = 15;
  back->position.y = 15;

  Floor* left = new Floor();
  left->rotation.z = PI / 2;
  left->position.y = 15;
  left->position.x = 15;
  left->rotation.x = PI /2;

  Floor* right = new Floor();
  right->rotation.z = -PI / 2;
  right->position.x = -15;
  right->position.y = 15;
  right->rotation.x = PI /2;

  ramp = new Floor();
  ramp->position.z = -30;
  ramp->anchor.z = -15;
  ramp->rotation.x = PI / 8;

  player = new Player();
  player->rotation.y = PI;

  meshes.push_back(back);
  meshes.push_back(floor);
  meshes.push_back(left);
  meshes.push_back(right);
  meshes.push_back(ramp);
  meshes.push_back(player);
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  OBB box = OBB(Vector3(0, 0, 0), Vector3(1));
  Matrix4 m = Matrix4::FromTranslation(0, 5-time/2, 0);
  box.update(m);
  player->position.z -= 0.08f;

  for(IGameObject *mesh : meshes)
    mesh->update(keyboard);
  for(IGameObject *mesh : meshes)
    mesh->draw(camera);

  bool yes = false;

  int i = 0;
  while (ramp->intersects(player) && i < 100) {
    player->position.y += 0.01f;
    player->updateBoundary();
    i++;
    yes = ramp->intersects(player);
  }

  printf("intersection: %s\n", yes ? "yes" : "no");

  time+=0.03f;
  RM->lightset[0].position.x = 11 * sin(time);
  RM->lightset[0].position.z = 11 * cos(time);
  RM->lightset[1].position.x = -11 * sin(time);
  RM->lightset[1].position.z = -11 * cos(time);
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);
}

bool Application::shouldClose() { return false; }
