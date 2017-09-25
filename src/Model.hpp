#pragma once
#include "run.hpp"

class Mesh {
 public:
  Mesh();
  Mesh(uint32_t count, std::string tex);
  ~Mesh();
  uint32_t indexCount;  // vertices count
  std::string texname;

 private:
};

class Model {
 public:
  Model(const std::string filename);
  ~Model();

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<Mesh> meshes;

 private:
};

static std::string GetBaseDir(const std::string& filepath) {
  if (filepath.find_last_of("/\\") != std::string::npos)
    return filepath.substr(0, filepath.find_last_of("/\\"));
  return "";
}
