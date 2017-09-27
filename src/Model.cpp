#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Mesh::Mesh() {}

Mesh::Mesh(uint32_t count, std::string tex) : indexCount(count), texname(tex) {}

Mesh::~Mesh() {}

Model::Model(const std::string filename) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;
  std::string basedir = GetBaseDir(filename);
  if (basedir.empty()) basedir = ".";
  basedir += "/";

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(),
                        basedir.c_str())) {
    std::cerr << "Cannot load obj: " << filename.c_str() << std::endl;
    return;
  }
  if (materials.empty()) {
    materials.push_back(tinyobj::material_t());
  }
  for (const auto& material : materials) {
    meshes.push_back(Mesh(0, basedir + material.ambient_texname));
  }

  struct Face {
    Vertex vertices[3];
    int32_t material_id;
  };

  std::vector<Face> faceList;
  for (const auto& shape : shapes) {
    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
      Face face;

      int material_id;
      material_id =
          f < shape.mesh.material_ids.size() ? shape.mesh.material_ids[f] : 0;
      if (material_id == -1) material_id = 0;
      face.material_id = material_id;
      for (size_t j = 0; j < 3; j++) {
        int vertex_index = shape.mesh.indices[(f * 3) + j].vertex_index;
        int normal_index = shape.mesh.indices[(f * 3) + j].normal_index;
        int texcoord_index = shape.mesh.indices[(f * 3) + j].texcoord_index;

        if (vertex_index != -1) {
          face.vertices[j].position = {attrib.vertices[3 * vertex_index + 0],
                                       attrib.vertices[3 * vertex_index + 1],
                                       attrib.vertices[3 * vertex_index + 2]};
        }
        if (texcoord_index != -1) {
          face.vertices[j].uv = {attrib.texcoords[2 * texcoord_index + 0],
                                 attrib.texcoords[2 * texcoord_index + 1]};
        }
      }

      faceList.push_back(face);
    }
  }
  // Sort vertices by material
  for (size_t material_id = 0; material_id < materials.size(); material_id++) {
    int vertexCount = 0;
    // meshes[material_id].vertexOffset = vertices.size();
    for (const auto& face : faceList) {
      if (face.material_id == material_id) {
        vertices.push_back(face.vertices[0]);
        vertices.push_back(face.vertices[1]);
        vertices.push_back(face.vertices[2]);
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        vertexCount += 3;
      }
    }
    meshes[material_id].indexCount = vertexCount;
  }
}

Model::~Model() {}
