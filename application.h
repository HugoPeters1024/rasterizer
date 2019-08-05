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
  ramp->rotation.x = PI / 4;

  Player* player = new Player();
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
  /*
  OBB box1 = OBB(Vector3(0, 0, 0), Vector3(1));
  OBB box2 = OBB(Vector3(0, 0, 0), Vector3(1));
  Matrix4 m1 = Matrix4::FromAxisRotations(0, 0, time);
  Matrix4 m2 = Matrix4::FromTranslation(-2.8, 0, 0) * m1;
  box2.update(m2);
  box1.update(m1);


  Line p = box1.project(Vector3(1, 0, 0));
  Line p2 = box2.project(Vector3(1, 0, 0));
  printf("projection1 from %f to %f, l = %f\n", p.a.x, p.b.x, abs(p.a.x - p.b.x));
  printf("projection1 from %f to %f\n", p2.a.x, p2.b.x);
  bool yes = box1.intersects(box2);
  yes = p.parallel_overlap(p2);
  */


  OBB box = OBB(Vector3(0, 0, 0), Vector3(1));
  Matrix4 m = Matrix4::FromTranslation(0, 5-time/2, 0);
  box.update(m);
  bool yes = floor->boundary.intersects(box);

  printf("floor: \n");
  for(const Vector3 &v : floor->boundary.getPoints())
    v.print();
  printf("---------\n");

  printf("box: \n");
  for(const Vector3 &v : box.getPoints())
    v.print();
  printf("---------\n");

  printf("intersection: %s\n", yes ? "yes" : "no");

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
