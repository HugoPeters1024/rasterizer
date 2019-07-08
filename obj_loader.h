#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include <stdio.h>
#include <vector>
#include <sstream>


struct vertex {
    std::vector<float> v;
    void normalize() {
        float magnitude = 0.0f;
        for (int i = 0; i < v.size(); i++)
            magnitude += pow(v[i], 2.0f);
        magnitude = sqrt(magnitude);
        for (int i = 0; i < v.size(); i++)
            v[i] /= magnitude;
    }
    vertex operator-(vertex v2) {
        vertex v3;
        if (v.size() != v2.v.size()) {
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
        } else {
            for (int i = 0; i < v.size(); i++)
                v3.v.push_back(v[i] - v2.v[i]);
        }
        return v3;
    }
    vertex cross(vertex v2) {
        vertex v3;
        if (v.size() != 3 || v2.v.size() != 3) {
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
        } else {
            v3.v.push_back(v[1]*v2.v[2]-v[2]*v2.v[1]);
            v3.v.push_back(v[2]*v2.v[0]-v[0]*v2.v[2]);
            v3.v.push_back(v[0]*v2.v[1]-v[1]*v2.v[0]);
        }
        return v3;
    }
};

struct face {
    std::vector<int> vertex;
    std::vector<int> texture;
    std::vector<int> normal;
};

class cObj {
  private:
    std::vector<vertex> vertices;
    std::vector<vertex> texcoords;
    std::vector<vertex> normals;
    std::vector<vertex> parameters;
    std::vector<face> faces;
  public:
    cObj(std::string filename);
    ~cObj();

  void renderBuffers(std::vector<float> &v_buf, std::vector<float> &n_buf) const;
};

cObj::cObj(std::string filename) {
    std::ifstream ifs(filename.c_str(), std::ifstream::in);
    std::string line, key;
    while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {
        key = "";
        std::stringstream stringstream(line);
        stringstream >> key >> std::ws;


        if (key == "v") { // vertex
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            vertices.push_back(v);
        } else if (key == "vp") { // parameter
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            parameters.push_back(v);
        } else if (key == "vt") { // texture coordinate
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            texcoords.push_back(v);
        } else if (key == "vn") { // normal
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            v.normalize();
            normals.push_back(v);
        } else if (key == "f") { // face
            face f; int v, t, n;
            while (!stringstream.eof()) {
                stringstream >> v >> std::ws;
                f.vertex.push_back(v-1);
                if (stringstream.peek() == '/') {
                    stringstream.get();
                    if (stringstream.peek() == '/') {
                        stringstream.get();
                        stringstream >> n >> std::ws;
                        f.normal.push_back(n-1);
                    } else {
                        stringstream >> t >> std::ws;
                        f.texture.push_back(t-1);
                        if (stringstream.peek() == '/') {
                            stringstream.get();
                            stringstream >> n >> std::ws;
                            f.normal.push_back(n-1);
                        }
                    }
                }
            }
            faces.push_back(f);
        } else {

        }
    }
    ifs.close();
    std::cout << "               Name: " << filename << std::endl;
    std::cout << "           Vertices: " << vertices.size() << std::endl;
    std::cout << "         Parameters: " << parameters.size() << std::endl;
    std::cout << "Texture Coordinates: " << texcoords.size() << std::endl;
    std::cout << "            Normals: " << normals.size() << std::endl;
    std::cout << "              Faces: " << faces.size() << std::endl << std::endl;
}

void cObj::renderBuffers(std::vector<float> &v_buf, std::vector<float> &n_buf) const
{
  for(face f : faces)
  {
    if (f.vertex.size() == 3 && f.normal.size() == 3)
    {
      for(int i = 0; i<3; i++)
      {
        int vi = f.vertex[i];
        int ni = f.normal[i];
        v_buf.push_back(vertices[vi].v[0]);
        v_buf.push_back(vertices[vi].v[1]);
        v_buf.push_back(vertices[vi].v[2]);

        n_buf.push_back(normals[ni].v[0]);
        n_buf.push_back(normals[ni].v[0]);
        n_buf.push_back(normals[ni].v[0]);
      }
    }
    else if (f.vertex.size() == 4 && f.normal.size() == 4)
    {
        // Extract the 4 vertices
        vertex v1 = vertices[f.vertex[0]];
        vertex v2 = vertices[f.vertex[1]];
        vertex v3 = vertices[f.vertex[2]];
        vertex v4 = vertices[f.vertex[3]];

        // Extract the 4 normals
        vertex n1 = normals[f.normal[0]];
        vertex n2 = normals[f.normal[1]];
        vertex n3 = normals[f.normal[2]];
        vertex n4 = normals[f.normal[3]];


        // Triangle 1
        v_buf.push_back(v1.v[0]);
        v_buf.push_back(v1.v[1]);
        v_buf.push_back(v1.v[2]);

        v_buf.push_back(v2.v[0]);
        v_buf.push_back(v2.v[1]);
        v_buf.push_back(v2.v[2]);

        v_buf.push_back(v3.v[0]);
        v_buf.push_back(v3.v[1]);
        v_buf.push_back(v3.v[2]);

        // Triangle 2
        v_buf.push_back(v1.v[0]);
        v_buf.push_back(v1.v[1]);
        v_buf.push_back(v1.v[2]);

        v_buf.push_back(v3.v[0]);
        v_buf.push_back(v3.v[1]);
        v_buf.push_back(v3.v[2]);

        v_buf.push_back(v4.v[0]);
        v_buf.push_back(v4.v[1]);
        v_buf.push_back(v4.v[2]);

        // Triangle 1 Normals
        n_buf.push_back(n1.v[0]);
        n_buf.push_back(n1.v[1]);
        n_buf.push_back(n1.v[2]);

        n_buf.push_back(n2.v[0]);
        n_buf.push_back(n2.v[1]);
        n_buf.push_back(n2.v[2]);

        n_buf.push_back(n3.v[0]);
        n_buf.push_back(n3.v[1]);
        n_buf.push_back(n3.v[2]);

        // Triangle 2 Normals
        n_buf.push_back(n1.v[0]);
        n_buf.push_back(n1.v[1]);
        n_buf.push_back(n1.v[2]);

        n_buf.push_back(n3.v[0]);
        n_buf.push_back(n3.v[1]);
        n_buf.push_back(n3.v[2]);

        n_buf.push_back(n4.v[0]);
        n_buf.push_back(n4.v[1]);
        n_buf.push_back(n4.v[2]);
    }
    else
    {
      logError("Cannot serialize a model that has %i vertices and %i normals per face", f.vertex.size(), f.normal.size());
      exit(4);
    }
  }
  logDebug("Expanded %zu faces to %zu coordinates", faces.size(), v_buf.size());
}

cObj::~cObj() { }

#endif
