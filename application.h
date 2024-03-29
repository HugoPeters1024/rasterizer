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
    std::vector<IGameObject*> objects;
    std::vector<ISolid*> solids;
    CameraObject* camera;
    Floor* ramp;
    Floor* floor;
    Floor* xramp;
    Player* player;
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

  camera = new CameraObject(1.25f);
  camera->pos.y = 17.5f;
  camera->pos.z = -15.5f;

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
  ramp->rotation.x = PI / 6;

  xramp = new Floor();
  xramp->position.z = -60;
  xramp->position.y = 0;

  player = new Player();
  player->rotation.y = PI;
  player->position.y += 2;

  objects.push_back(back);
  objects.push_back(floor);
  objects.push_back(left);
  objects.push_back(right);
  objects.push_back(ramp);
  objects.push_back(xramp);
  objects.push_back(player);

  solids.push_back(back);
  solids.push_back(floor);
  solids.push_back(left);
  solids.push_back(right);
  solids.push_back(ramp);
  solids.push_back(xramp);
  solids.push_back(camera);
  solids.push_back(player);
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  xramp->position.y += 0.1f;
  if (xramp->position.y > 50)
    xramp->position.y = 0;
  xramp->rotation.x += 0.01f;
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);
  camera->drawBoundary(camera);

  for(IGameObject *obj : objects)
    obj->update(keyboard);

  for(ISolid *obj : solids) {
      Vector3 normal;
      float dis;
      if (obj != player && player->intersects(obj, &normal, &dis)) {
        player->onCollision(obj, normal, dis);
      }
      if (obj != camera && camera->intersects(obj, &normal, &dis)) {
        camera->onCollision(obj, normal, dis);
    }
  }

  for(IGameObject *obj : objects)
    obj->draw(camera);


  time+=0.03f;
  RM->lightset[0].position.x = 11 * sin(time);
  RM->lightset[0].position.z = 11 * cos(time);
  RM->lightset[1].position.x = -11 * sin(time);
  RM->lightset[1].position.z = -11 * cos(time);
}

bool Application::shouldClose() { return false; }
