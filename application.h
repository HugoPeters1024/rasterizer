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

  auto xramp = new Floor();
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
  solids.push_back(player);
  player->velocity.y = 0;
  player->velocity.z = 0.1;
}

void Application::loop(int w, int h, Keyboard* keyboard)
{
  player->velocity.z -= 0.0015f;
  for(IGameObject *obj : objects)
    obj->update(keyboard);

  /*
  for(ISolid *obj : solids)
    for(ISolid *other : solids) {
      if (obj == other) continue;
      Vector3 normal;
      if (obj->intersects(other, &normal)) {
        if (normal.sq_length() - 1 > 1e-6) return;
        normal.print();
        obj->onCollision(other, normal);
      }
    }
    */

  /*
  Vector3 normal;
  if (player->intersects(ramp, &normal)) {
    normal.print();
  }
  */

  /*
  float yy = 5 - time / 5;
  OBB box1 = OBB(Vector3(0), Vector3(1));
  OBB box2 = OBB(Vector3(0), Vector3(1));
  Matrix4 t = Matrix4::FromTranslation(0, yy, 0);
  Matrix4 r = Matrix4::FromAxisRotations(0, 0, time/8);
  box2.update(t);
  Vector3 normal;
  float dist;
  bool yes = box1.intersects(box2, &normal, &dist);
  if (yes) normal.print();
  */

  //auto l1 = box1.project(Vector3(0, 0, -1));
  //auto l2 = box1.project(Vector3(0, 0, -1));
  //printf("box1 from %f to %f\n", l1.a, l1.b);
 // printf("box2 from %f to %f\n", l2.a, l2.b);

  //box1.draw(camera);
  //box2.draw(camera);

  /*
  Line l = Line(Vector3(0, 0, -1));
  printf("line from %f to %f\n", l.a, l.b);
  l.consume(Vector3(0, 0, 1));
  printf("line from %f to %f\n", l.a, l.b);
  l.consume(Vector3(0, 0, -1));
  printf("line from %f to %f\n", l.a, l.b);
  printf("------\n");
  */



  /*
  auto l1 = Line(Vector3(1, 0, 0), 0, 3);
  auto l2 = Line(Vector3(1, 0, 0), -1+time/10, 0.01+time/10);
  float overlap2;
  l1.parallel_overlap(l2, &overlap2);
  printf("(%f -- %f)  and (%f -- %f)\n", l1.a, l1.b, l2.a, l2.b);
  printf("overlap2  %f\n", overlap2);
  */


  for(ISolid *obj : solids) {
    Vector3 normal;
    float dis;
    if (obj == player) continue;
    if (player->intersects(obj, &normal, &dis)) {
      player->onCollision(obj, normal, dis);
    }
  }

  for(IGameObject *obj : objects)
    obj->draw(camera);

  /*
  int i=0;
  while (ramp->intersects(player) && i < 100) {
    player->position.y += 0.01f;
    player->updateBoundary();
    i++;
  }
  */

  time+=0.03f;
  RM->lightset[0].position.x = 11 * sin(time);
  RM->lightset[0].position.z = 11 * cos(time);
  RM->lightset[1].position.x = -11 * sin(time);
  RM->lightset[1].position.z = -11 * cos(time);
  float ratio = w / (float)h;
  camera->update(ratio, keyboard);
}

bool Application::shouldClose() { return false; }
