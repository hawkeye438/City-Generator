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
	bool mouseButtonRightDown, mouseButtonMiddleDown, mouseButtonLeftDown;
	double xpos_click, ypos_click;
	float mv_distance;
	float terrain_bounds;
	float yaw, pitch;
	vector<BoundingBox*> boxes;
	float min_xd, max_xd, min_zd, max_zd;

public:
	Camera() {}
	Camera(glm::vec3 e, glm::vec3 c, glm::vec3 u): eye(e), center(c), up(u) {
		mouseButtonRightDown = false;
		mouseButtonMiddleDown = false;
		mouseButtonLeftDown = false;
		xpos_click = 0.0f;
		ypos_click = 0.0f;
		mv_distance = 0.1f;
		terrain_bounds = 0.1f;
		yaw = -90.0f;	
		pitch = 0.0f;

		min_xd = 0.0f;
		max_xd = 0.0f;
		min_zd = 0.0f;
		max_zd = 0.0f;
	}
	~Camera() {}

	void setCameraBoxes(vector<BoundingBox*> b) { boxes = b; }
	void setPerspective(float fov, GLfloat aspect, GLfloat near, GLfloat far, GLuint projectionLoc);
	void setLookAt(glm::mat4 &view_matrix, GLuint viewMatrixLoc);

	void checkCollision(glm::vec3 point, float offset, int value);
	void checkTerrainCollision();
	void cameraKeys(GLFWwindow* window, int key, int scancode, int action, int mode);
	void cameraMouse(GLFWwindow* window, double xpos, double ypos);
	void cameraMouseButtons(GLFWwindow* window, int button, int action, int mods);

	void setLoopCoord(int x, int z);
	void checkLoopPos();

};
#endif