#include "Shader.h"

Shader::Shader(const std::string vertex_shader_path, const std::string fragment_shader_path) {
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);

	// Check for compile time errors
	checkCompileErrors(vertexShader, "VERTEX");

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);

	// Check for compile time errors
	checkCompileErrors(fragmentShader, "FRAGMENT");

	// Link shaders
	Id = glCreateProgram();
	glAttachShader(Id, vertexShader);
	glAttachShader(Id, fragmentShader);
	glLinkProgram(Id);

	// Check for linking errors
	checkCompileErrors(Id, "PROGRAM");

	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {

}

void Shader::use() {
	glUseProgram(Id);
}

GLuint Shader::getShaderId()
{
	return Id;
}

void Shader::checkCompileErrors(GLuint value, std::string type) {
	GLint success;
	GLchar infoLog[512];

	if (type == "VERTEX") {
		glGetShaderiv(value, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(value, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (type == "FRAGMENT") {
		glGetShaderiv(value, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(value, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (type == "PROGRAM") {
		glGetProgramiv(value, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(value, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}