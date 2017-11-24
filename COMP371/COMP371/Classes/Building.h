#pragma once
#ifndef BUILDING_H
#define BUILDING_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include "CubePrimitive.h"
#include "Utility.h"
#include "BoundingBox.h"

using namespace std;

class Building
{
	vector<GLuint> buildings;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	glm::vec3 min, max;
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;

public:
	Building() {}
	~Building() {}

	void createBuildings(int num);
	void render(vector<BoundingBox*> &b, GLuint transformLoc, GLuint texture_option, GLuint texture_matrix, GLuint scale_UV, int city_dim, vector<glm::vec3> building_scales, vector<int> random_texture);
};
#endif