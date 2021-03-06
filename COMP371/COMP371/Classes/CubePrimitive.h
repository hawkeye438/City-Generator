#pragma once
#ifndef CUBEPRIMITIVE_H
#define CUBEPRIMITIVE_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include "../objloader.hpp"

using namespace std;

class CubePrimitive {

public:
	static void loadCube(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &UVs);
	static void bindCube(GLuint &VAO, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &UVs);
};
#endif