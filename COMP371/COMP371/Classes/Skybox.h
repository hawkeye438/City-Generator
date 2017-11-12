#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include "..\soil\SOIL.h"

using namespace std;

class Skybox
{
	GLuint skybox_VAO;
	GLuint skybox_VBO;
public:
	Skybox();
	~Skybox();

	void loadSkyBox(const char* right, const char* left, const char* top,
		const char* bottom, const char* back, const char* front, GLuint shader);

	void render(glm::mat4 &view_matrix, GLuint viewMatrixLoc, GLuint drawing_skybox_id);

	GLuint loadCubemap(vector<const char*> faces);
};
#endif