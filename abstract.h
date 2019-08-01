#ifndef ABSTRACT_H
#define ABSTRACT_H

// Defines abstract types
class IMesh {
  public:
    vec3 position, rotation, anchor;
    virtual void draw(const Camera* camera) const = 0;
    virtual void update(Keyboard* keyboard) = 0;
    virtual void getMvp(mat4x4 m) const = 0;
};

#endif
