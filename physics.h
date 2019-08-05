#include "mesh.h"

class OBB {
private:
  Vector3 dimensions, pos;
  
public: std::array<Vector3, 8> points;
  Matrix4 m;
  Line project(const Vector3 &ax) const {
    Line line;
    bool first = true;
    for(const Vector3 &v : getPoints()) {
      Vector3 pv = ax * Vector3::dot(v, ax);
      if (first) { line.a = pv; line.b = pv; first = false; } else
      line.consume(pv);
    }
    return line;
  }
  void draw(const Camera* cam, Matrix4 m) const { 
    Matrix4 s = Matrix4::FromScale(dimensions * 2);
    Matrix4 p = Matrix4::FromTranslation(pos);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mesh->draw(cam, m * p * s);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

public:
  static IMesh* mesh;
  OBB(Vector3 pos, Vector3 dimensions) : m(Matrix4::Identity()), dimensions(dimensions), pos(pos) {

    Vector3 ux = Vector3(1, 0, 0);
    Vector3 uy = Vector3(0, 1, 0);
    Vector3 uz = Vector3(0, 0, 1);

    points = std::array<Vector3, 8>();
    points[0] = pos + dimensions.x * ux - dimensions.y * uy + dimensions.z * uz;
    points[1] = pos - dimensions.x * ux - dimensions.y * uy + dimensions.z * uz;
    points[2] = pos - dimensions.x * ux - dimensions.y * uy - dimensions.z * uz;
    points[3] = pos + dimensions.x * ux - dimensions.y * uy - dimensions.z * uz;

    points[4] = pos + dimensions.x * ux + dimensions.y * uy + dimensions.z * uz;
    points[5] = pos - dimensions.x * ux + dimensions.y * uy + dimensions.z * uz;
    points[6] = pos - dimensions.x * ux + dimensions.y * uy - dimensions.z * uz;
    points[7] = pos + dimensions.x * ux + dimensions.y * uy - dimensions.z * uz;
  }
  std::array<Vector3, 8> getPoints() const {
    std::array<Vector3, 8> ret;
    for(int i=0; i<8; i++) {
      ret[i] = (m * Vector4(points[i], 1)).xyz();
    }
    return ret;
  };
  std::array<Vector3, 3> getNormals() const {
    std::array<Vector3, 3> ret;
    auto vs = getPoints();
    ret[0] = (vs[1] - vs[0]).normalize();
    ret[1] = (vs[3] - vs[0]).normalize();
    ret[2] = (vs[4] - vs[0]).normalize();
    return ret;
  }
  bool intersects(const OBB &o) const {
     auto nleft  = o.getNormals();
     auto nright = getNormals();
     Vector3 axis[6];
     axis[0] = nleft[0];
     axis[1] = nleft[1];
     axis[2] = nleft[2];
     axis[3] = nright[0];
     axis[4] = nright[1];
     axis[5] = nright[2];
     for(const Vector3 &ax : axis) {
       Line p1 = project(ax);
       Line p2 = o.project(ax);
       if (!p1.parallel_overlap(p2)) return false;
     }
     return true;
  }

  void update(const Matrix4& mvp) {
    m = mvp;
  }
};

IMesh* OBB::mesh;
