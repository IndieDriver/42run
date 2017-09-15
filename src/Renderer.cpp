#include "Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Renderer::Renderer(void) {}

Renderer::Renderer(Renderer const &src) { *this = src; }

Renderer::~Renderer(void) {}

Renderer &Renderer::operator=(Renderer const &rhs) {
  if (this != &rhs) {
    this->view = rhs.view;
    this->proj = rhs.proj;
    this->_renderAttribs = rhs._renderAttribs;
  }
  return (*this);
}

void Renderer::addRenderAttrib(const RenderAttrib &renderAttrib) {
  this->_renderAttribs.push_back(renderAttrib);
}

void Renderer::draw() {
  std::sort(_renderAttribs.begin(), _renderAttribs.end());
  int shader_id = -1;
  int texture_id = -1;
  for (const auto &attrib : this->_renderAttribs) {
    if (attrib.shader != shader_id) {
      glUseProgram(attrib.shader);
      shader_id = attrib.shader;
    }
    if (shader_id != -1) {
      if (attrib.texture != nullptr) {
        if (attrib.texture->id != texture_id) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, attrib.texture->id);
          texture_id = attrib.texture->id;
        }
      } else {
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      glm::mat4 mvp = this->proj * this->view * attrib.transforms[0];
      glUniformMatrix4fv(glGetUniformLocation(attrib.shader, "MVP"), 1,
                         GL_FALSE, static_cast<GLfloat *>(glm::value_ptr(mvp)));
      if (attrib.vao->ebo != -1) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attrib.vao->ebo);
        glBindVertexArray(attrib.vao->vao);
        glDrawElements(GL_TRIANGLES, attrib.vao->indices_size, GL_UNSIGNED_INT,
                       static_cast<void *>(0));
      } else {
        glBindVertexArray(attrib.vao->vao);
        glDrawArrays(GL_TRIANGLES, 0, attrib.vao->vertices_size);
      }
    }
  }
}

void Renderer::flush() { this->_renderAttribs.clear(); }

void Renderer::printRenderAttribs() {
  std::cout << "------------" << std::endl;
  for (const auto &attrib : this->_renderAttribs) {
    std::cout << attrib.shader << " | " << attrib.vao->vao << std::endl;
  }
  std::cout << "------------" << std::endl;
}

VAO::VAO(std::vector<Vertex> vertices) {
  GLuint vbo;
  this->vao = -1;
  this->ebo = -1;
  this->vertices_size = vertices.size();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, this->vertices_size * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, uv));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

VAO::VAO(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
  GLuint vbo;
  this->vertices_size = vertices.size();
  this->indices_size = indices.size();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, this->vertices_size * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, uv));

  glGenBuffers(1, &this->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Texture::Texture(std::string filename) : id(-1) {
  int texWidth;
  int texHeight;
  int texChannels;
  stbi_set_flip_vertically_on_load(true);
  stbi_uc *pixels = stbi_load(filename.c_str(), &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
  if (pixels != nullptr) {
    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
  } else {
    std::cerr << "Invalid texture file: " << filename << std::endl;
  }
}
