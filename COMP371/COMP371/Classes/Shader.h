#ifndef SHADER_H
#define SHADER_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include <iostream>
#include <string>
#include <fstream>

class Shader
{
	GLuint Id;
	
	void checkCompileErrors(GLuint value, std::string type);
public:
	Shader(const std::string vertex_shader_path, const std::string fragment_shader_path);
	~Shader();
	void use();
	GLuint getShaderId();

};
#endif