#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <algorithm>
#include <map>
#include "Shader.hpp"
#include "env.hpp"
#include "run.hpp"

struct Character {
  GLuint textureID;      // ID handle of the glyph texture
  glm::ivec2 size;       // Size of glyph
  glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
  GLuint advanceOffset;  // Offset to advance to next glyph
};

struct Texture {
  Texture(std::string filename);
  GLuint id;
};

struct VAO {
  VAO(std::vector<Vertex> vertices);
  VAO(std::vector<Vertex> vertices, std::vector<GLuint> indices);
  GLuint vao;
  GLuint ebo;
  GLsizei vertices_size;
  GLsizei indices_size;
};

struct RenderAttrib {
  VAO* vao;
  GLuint shader;
  std::vector<glm::mat4> transforms;
  Texture* texture;

  bool operator<(const struct RenderAttrib& rhs) const {
    if (this->shader == rhs.shader) {
      return (this->vao->vao < rhs.vao->vao);
    } else if (this->shader < rhs.shader) {
      return this->shader < rhs.shader;
    } else {
      return this->shader > rhs.shader;
    }
  }
};

class TextRenderer {
 public:
  TextRenderer(void);
  void renderText(float pos_x, float pos_y, float scale, std::string text,
                  glm::vec3 color, glm::mat4 ortho);

 private:
  std::map<GLchar, Character> _characters;
  GLuint _vao;
  GLuint _vbo;
  GLuint _shader_id;
};

class Renderer {
 public:
  Renderer(int width, int height);
  Renderer(Renderer const& src);
  virtual ~Renderer(void);
  Renderer& operator=(Renderer const& rhs);
  void addRenderAttrib(const RenderAttrib& renderAttrib);
  void renderText(float pos_x, float pos_y, float scale, std::string text,
                  glm::vec3 color);
  void draw();
  void flush();
  void printRenderAttribs();

  TextRenderer textRenderer;
  glm::mat4 view;
  glm::mat4 proj;

 private:
  int _width;
  int _height;
  Renderer(void);
  std::vector<RenderAttrib> _renderAttribs;
};
