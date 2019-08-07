#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "vec.h"
#include "keyboard.h"
#include "camera.h"
#include "resources.h"
#include "physics.h"

class IGameObject {
public:
  virtual void update(Keyboard* keyboard) = 0;
  virtual void draw(Camera* camera) const = 0;
};

class ISolid {
  private:
    OBB boundary;
  protected:
    void updateBoundary(Matrix4 m) { boundary.update(m); }
  public:
    virtual void onCollision(const ISolid* other, Vector3 normal) {}
    virtual void updateBoundary() = 0; 
    ISolid() : boundary(OBB(Vector3(0), Vector3(0))) {}
    ISolid(OBB boundary) : boundary(boundary) {}
    bool intersects(const ISolid* o, Vector3* normal = 0) {
      return boundary.intersects(o->boundary, normal);
    }
    void drawBoundary(const Camera* cam, Matrix4 m) const { boundary.draw(cam, m); }
};

class IMeshObject : public IGameObject {
public:
  Vector3 position, rotation, anchor, scale;
protected:
  IMeshObject() : scale(Vector3(1)) {}
  IMeshObject(float scale) : scale(scale) {}
  virtual Matrix4 getMvp() const {
    Matrix4 t = Matrix4::FromTranslation(position);
    Matrix4 r = Matrix4::FromAxisRotations(rotation);
    Matrix4 s = Matrix4::FromScale(scale);
    Matrix4 a1 = Matrix4::FromTranslation(anchor);
    Matrix4 a2 = Matrix4::FromTranslation(-anchor);
    return t * a2 * r * a1 * s;
  }
};

class SolidMesh : public IMeshObject, public ISolid {
  public:
    SolidMesh(float scale, OBB boundary) : IMeshObject(scale), ISolid(boundary) {}
    void updateBoundary() override { ISolid::updateBoundary(getMvp()); }
};

class Floor : public SolidMesh {
public:
  Floor() : SolidMesh(15, OBB(Vector3(0), Vector3(1, 0.01, 1))) {}
  static IMesh* mesh;
  void onCollision(const ISolid* other, Vector3 normal) override {
  }
  void update(Keyboard* keyboard) override {
    updateBoundary();
  }
  void draw(Camera* camera) const override {
    mesh->draw(camera, getMvp(), 0.8f);
  }
};
IMesh* Floor::mesh;

class Player : public SolidMesh {
private:
public:
  Vector3 velocity;
  Player() : SolidMesh(1, OBB(Vector3(0, 9, 0), Vector3(2, 9, 2))) {}
  static IMesh* mesh;
  void onCollision(const ISolid* other, Vector3 normal) override {
    normal.print();
    //position += normal;
    velocity = Vector3::reflect(velocity, normal) * 0.97;
    //normal.print();
    //velocity = normal;
    updateBoundary();
  }
  void update(Keyboard* keyboard) override {
    position += velocity;
    velocity.y -= 0.004f;
    velocity.z += 0.0001f;
    updateBoundary();
  };
  void draw(Camera* camera) const override {
    Matrix4 mvp = getMvp();
    mesh->draw(camera, mvp);
    drawBoundary(camera, mvp);
  };
};
IMesh* Player::mesh;

void gameInit(ResourceManager* RM)
{
  Floor::mesh = RM->getMesh("floor");
  Player::mesh = RM->getMesh("player");
  OBB::mesh = RM->getMesh("cube");
}

#endif
