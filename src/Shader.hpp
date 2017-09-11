#pragma once
#include <fstream>
#include <string>
#include "env.hpp"

class Shader {
public:
	GLuint id = 0;
	Shader(std::string vertexFilename, std::string fragFilename);
	void use() const{
		glUseProgram(id);
	}

private:
	GLuint compileShader(std::string filename, GLuint shaderType);
	GLuint linkShaders(GLuint vertexID, GLuint fragID);
};

void    printShaderError(GLuint shade, std::string filename);
void    printLinkError(GLuint program, std::string vextexFilename, std::string fragmentFilename);
