#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "vec.h"
#include "keyboard.h"
#include "camera.h"
#include "resources.h"

class IGameObject {
public:
  virtual void update(Keyboard* keyboard) = 0;
  virtual void draw(Camera* camera) const = 0;
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
    return t * s * a2 * r * a1;
  }
};

class Floor : public IMeshObject {
public:
  Floor() : IMeshObject(15) {}
  static IMesh* mesh;
  void update(Keyboard* keyboard) override {};
  void draw(Camera* camera) const override {
    mesh->draw(camera, getMvp(), 0.8f);
  };
};
IMesh* Floor::mesh;

class Player : public IMeshObject {
public:
  static IMesh* mesh;
  void update(Keyboard* keyboard) override {
  };
  void draw(Camera* camera) const override {
    mesh->draw(camera, getMvp());
  };
};
IMesh* Player::mesh;

void gameInit(ResourceManager* RM)
{
  Floor::mesh = RM->getMesh("floor");
  Player::mesh = RM->getMesh("player");
}

#endif
