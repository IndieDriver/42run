#pragma once
#include <fstream>
#include <string>
#include "env.hpp"

class Shader {
 public:
  Shader(std::string vertexFilename, std::string fragFilename);
  Shader(Shader const &src);
  virtual ~Shader(void);
  Shader &operator=(Shader const &rhs);

  GLuint id;
  void use() const;

 private:
  Shader(void);
  GLuint compileShader(std::string filename, GLuint shaderType);
  GLuint linkShaders(GLuint vertexID, GLuint fragID);
};

void printShaderError(GLuint shade, std::string filename);
void printLinkError(GLuint program, std::string vextexFilename,
                    std::string fragmentFilename);
