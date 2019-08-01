#ifndef ABSTRACT_H
#define ABSTRACT_H

// Defines abstract types
class IMesh {
  public:
    IMesh();
    vec3 position, rotation, anchor;
    float scale;
    virtual void draw(const Camera* camera) const = 0;
    virtual void update(Keyboard* keyboard) = 0;
    void getMvp(mat4x4 f) const;
};

IMesh::IMesh()
{
  vec3_zero(position);
  vec3_zero(rotation);
  vec3_zero(anchor);
  scale = 1;
}

void IMesh::getMvp(mat4x4 f) const
{
  mat4x4 i, rx, ry, rz, t, ta, ta_r, s;
  mat4x4_identity(i);

  mat4x4_translate(t, position[0], position[1], position[2]);
  mat4x4_translate(ta, anchor[0], anchor[1], anchor[2]);
  mat4x4_translate(ta_r, -anchor[0], -anchor[1], -anchor[2]);
  mat4x4_scale_aniso(s, i, scale, scale, scale);
  mat4x4_rotate_X(rx, i, rotation[0]);
  mat4x4_rotate_Y(ry, i, rotation[1]);
  mat4x4_rotate_Z(rz, i, rotation[2]);

  mat4x4_mul(f, s, i);  // scale
  mat4x4_mul(f, ta, f); // translate anchor
  mat4x4_mul(f, rx, f); // rotate
  mat4x4_mul(f, ry, f);
  mat4x4_mul(f, rz, f);
  mat4x4_mul(f, ta_r, f); // reverse translate anchor
  mat4x4_mul(f, t, f);  // translate
}

#endif
