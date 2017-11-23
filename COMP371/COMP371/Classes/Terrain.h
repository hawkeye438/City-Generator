#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include <iostream>
#include "Utility.h"
#include "BoundingBox.h"

using namespace std;

class Terrain
{
	GLuint mesh_VBO, mesh_VAO, mesh_EBO, mesh_NORMAL, mesh_UV;
	int number_of_indices;
	int mesh_width, mesh_height;

public:
	Terrain();
	~Terrain();

	void loadTerrain(int width, int height);
	void render(GLuint transform_loc, float scale, float y_offset);
};
#endif