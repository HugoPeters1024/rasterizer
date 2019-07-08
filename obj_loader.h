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
  public:
    std::vector<vertex> vertices;
    std::vector<vertex> texcoords;
    std::vector<vertex> normals;
    std::vector<vertex> parameters;
    std::vector<face> faces;

    cObj(std::string filename);
    ~cObj();

    std::vector<float> renderVertices();
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

std::vector<float> cObj::renderVertices()
{
  std::vector<float> result;
  for(face f : faces)
  {
    if (f.vertex.size() == 3)
    {
      for(int i : f.vertex)
      {
        result.push_back(vertices[i].v[0]);
        result.push_back(vertices[i].v[1]);
        result.push_back(vertices[i].v[2]);
      }
    }
    else if (f.vertex.size() == 4)
    {
        vertex v1 = vertices[f.vertex[0]];
        vertex v2 = vertices[f.vertex[1]];
        vertex v3 = vertices[f.vertex[2]];
        vertex v4 = vertices[f.vertex[3]];
        // Triangle 1
        result.push_back(v1.v[0]);
        result.push_back(v1.v[1]);
        result.push_back(v1.v[2]);

        result.push_back(v2.v[0]);
        result.push_back(v2.v[1]);
        result.push_back(v2.v[2]);

        result.push_back(v3.v[0]);
        result.push_back(v3.v[1]);
        result.push_back(v3.v[2]);

        // Triangle 2
        result.push_back(v1.v[0]);
        result.push_back(v1.v[1]);
        result.push_back(v1.v[2]);

        result.push_back(v3.v[0]);
        result.push_back(v3.v[1]);
        result.push_back(v3.v[2]);

        result.push_back(v4.v[0]);
        result.push_back(v4.v[1]);
        result.push_back(v4.v[2]);
    }
    else
    {
      logError("Cannot serialize a model that has %i result per face", f.vertex.size());
      exit(4);
    }
  }
  logDebug("Expanded %i faces to %i coordinates", faces.size(), result.size());
  return result;
}

cObj::~cObj() { }

#endif
