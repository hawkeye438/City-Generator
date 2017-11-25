#pragma once
#ifndef LIGHTING_H
#define LIGHTING_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"
#include <vector>
#include <string>

using namespace std;

class Lighting
{
	int num_of_lights;
	glm::vec3 pointLightPositions[26];
	glm::vec3 pointLightAmbient[26];

public:
	Lighting() {}
	~Lighting() {}

	void setLightPositions(int num_of_rows, int num_of_cols);
	void setLightAttributes();
	void render(Shader &shader_program, glm::vec3 positions[], glm::vec3 ambient[]);

	glm::vec3* getLightPositions();
	glm::vec3* getLightAmbient();

};

#endif