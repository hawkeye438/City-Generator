#pragma once
#ifndef LIGHTING_H
#define LIGHTING_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"
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

public:
	Lighting() {}
	~Lighting() {}

	//Point lights
	glm::vec3 pointLightPositions[26];
	glm::vec3 pointLightAmbient[26];
	glm::vec3 pointLightDiffuse[26];
	glm::vec3 pointLightSpecular[26];
	float pointAmbStrength[26];

	//Direction light
	glm::vec3 moonLightDirection;
	glm::vec3 moonLightAmbient;
	glm::vec3 moonLightDiffuse;
	glm::vec3 moonLightSpecular;
	float moonAmbStrength;

	//City Lights
	void setCityLightPositions(int num_of_rows, int num_of_cols);
	void setCityLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void setCityAmbientStrength(float strength);
	void render(Shader &shader_program);

	//Sun Light
	void setSunLightPosition(glm::vec3 position);
	void setSunLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void setSunAmbientStrength(float strength);

	glm::vec3 getSunLightPosition();

	//Moon Light
	void setMoonLightDirection(glm::vec3 position);
	void setMoonLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void setMoonAmbientStrength(float strength);

};

#endif