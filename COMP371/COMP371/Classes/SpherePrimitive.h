#pragma once
#ifndef SPHEREPRIMITIVE_H
#define SPHEREPRIMITIVE_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include "../objloader.hpp"

using namespace std;

class SpherePrimitive {

public:
	static void bindSphere(GLuint &VAO, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &UVs);
};

#endif