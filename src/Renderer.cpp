#include "Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Renderer::Renderer(void) {}

Renderer::Renderer(int width, int height) : _width(width), _height(height) {}

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
  //printRenderAttribs();
  int shader_id = -1;
  int texture_id = -1;
  for (const auto &attrib : this->_renderAttribs) {
    if (attrib.shader > 0 && attrib.shader != shader_id) {
      glUseProgram(attrib.shader);
      shader_id = attrib.shader;
    }
    if (shader_id > 0 && attrib.vao != nullptr) {
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

void Renderer::renderText(float pos_x, float pos_y, float scale,
                          std::string text, glm::vec3 color) {
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->_width),
                                    0.0f, static_cast<float>(this->_height));
  textRenderer.renderText(pos_x, pos_y, scale, text, color, projection);
}

void Renderer::flush() { this->_renderAttribs.clear(); }

void Renderer::printRenderAttribs() {
  std::cout << "------------" << std::endl;
  for (const auto &attrib : this->_renderAttribs) {
    std::cout << "shader: " << attrib.shader;
    if (attrib.vao != nullptr) {
      std::cout << " | vao:" << attrib.vao->vao;
    }
    if (attrib.texture != nullptr) {
      std::cout << " | texture:" << attrib.texture->id;
    }
    std::cout << std::endl;
  }
  std::cout << "------------" << std::endl;
}

int Renderer::getScreenWidth() { return (this->_width); }

int Renderer::getScreenHeight() { return (this->_height); }

TextRenderer::TextRenderer(void) {
  Shader shader("shaders/text.vert", "shaders/text.frag");
  this->_shader_id = shader.id;

  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cout << "Could not init FreeType Library" << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    std::cout << "Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "Failed to load Glyph" << std::endl;
      continue;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<GLuint>(face->glyph->advance.x)};
    this->_characters.insert(std::pair<GLchar, Character>(c, character));
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &this->_vao);
  glGenBuffers(1, &this->_vbo);

  glBindVertexArray(this->_vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::renderText(float pos_x, float pos_y, float scale,
                              std::string text, glm::vec3 color,
                              glm::mat4 ortho) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(_shader_id);
  glUniformMatrix4fv(glGetUniformLocation(_shader_id, "proj"), 1, GL_FALSE,
                     glm::value_ptr(ortho));
  glUniform3fv(glGetUniformLocation(_shader_id, "text_color"), 1,
               glm::value_ptr(color));
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->_vao);
  for (char &c : text) {
    if (static_cast<int>(c) > 0 && static_cast<int>(c) < 128) {
      Character ch = this->_characters[static_cast<int>(c)];
      GLfloat xpos = pos_x + ch.bearing.x * scale;
      GLfloat ypos = pos_y - (ch.size.y - ch.bearing.y) * scale;

      GLfloat w = ch.size.x * scale;
      GLfloat h = ch.size.y * scale;
      GLfloat vertices[6][4] = {
          {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
          {xpos + w, ypos, 1.0, 1.0},

          {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
          {xpos + w, ypos + h, 1.0, 0.0}};
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      pos_x += (ch.advanceOffset >> 6) * scale;
    }
  }
  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
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

  calc_aabb(vertices);
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

  calc_aabb(vertices);
}

void VAO::calc_aabb(std::vector<Vertex> vertices) {
  for (const auto &vertex : vertices) {
    if (vertex.position.x < aabb_min.x) aabb_min.x = vertex.position.x;
    if (vertex.position.x > aabb_max.x) aabb_max.x = vertex.position.x;
    if (vertex.position.y < aabb_min.y) aabb_min.y = vertex.position.y;
    if (vertex.position.y > aabb_max.y) aabb_max.y = vertex.position.y;
    if (vertex.position.z < aabb_min.z) aabb_min.z = vertex.position.z;
    if (vertex.position.z > aabb_max.z) aabb_max.z = vertex.position.z;
  }
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
