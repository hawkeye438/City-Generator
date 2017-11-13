#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include "BoundingBox.h"

using namespace std;

class Camera {
	glm::vec3 eye, center, up;
	float rotate_x, rotate_y, rotate_z;
	vector<BoundingBox*> boxes;

public:
	Camera() {}
	Camera(glm::vec3 e, glm::vec3 c, glm::vec3 u, vector<BoundingBox*> b): eye(e), center(c), up(u), boxes(b) {
		rotate_x = 0.0f;
		rotate_y = 0.0f;
		rotate_z = 0.0f;
	}
	~Camera() {}

	void setPerspective(float fov, GLfloat aspect, GLfloat near, GLfloat far, GLuint projectionLoc);
	void setLookAt(glm::mat4 &view_matrix, GLuint viewMatrixLoc);
	void rotateCamera(glm::mat4 &view_matrix, GLuint viewMatrixLoc);
	
	void checkCollision(glm::vec3 point, float offset, int value);
	void cameraKeys(GLFWwindow* window, int key, int scancode, int action, int mode);
};
#endif