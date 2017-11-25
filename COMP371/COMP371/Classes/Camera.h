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
#include <random>
#include "BoundingBox.h"

using namespace std;

class Camera {
	enum fogMode {
		LINEAR = 1,
		EXP,
		EXPSQUARED,
		SCATTERING,
		NONE
	};

	glm::vec3 eye, center, up;
	bool mouseButtonRightDown, mouseButtonMiddleDown, mouseButtonLeftDown;
	double xpos_click, ypos_click;
	float mv_distance;
	float terrain_bounds;
	float yaw, pitch;
	vector<BoundingBox*> boxes;
	float min_xd, max_xd, min_zd, max_zd;

	//buildings
	vector<glm::vec3> building_scales;
	vector<int> random_texture;
	std::random_device rd; // create object for seeding
	std::mt19937 engine; // create engine and seed it
	int total_buildings;

	// Fog attributes
	fogMode currentFogMode;
	bool fogDebugValue;
	float fogStart;
	float fogEnd;
	float fogDensity;
	GLuint fog_option;
	GLuint fog_debug;
	GLuint fog_start;
	GLuint fog_end;
	GLuint fog_density;

public:
	Camera() {}
	Camera(glm::vec3 e, glm::vec3 c, glm::vec3 u, const GLuint fO, const GLuint fDbg, const GLuint fS, const GLuint fE, const GLuint fD) :
		eye(e), center(c), up(u), fog_option(fO), fog_debug(fDbg), fog_start(fS), fog_end(fE), fog_density(fD)
	{
		mouseButtonRightDown = false;
		mouseButtonMiddleDown = false;
		mouseButtonLeftDown = false;
		xpos_click = 0.0f;
		ypos_click = 0.0f;
		mv_distance = 1.0f;
		terrain_bounds = 0.1f;
		yaw = -90.0f;	
		pitch = 0.0f;

		min_xd = 0.0f;
		max_xd = 0.0f;
		min_zd = 0.0f;
		max_zd = 0.0f;

		engine = mt19937(rd());
		total_buildings = 0;

		currentFogMode = LINEAR;
		fogDebugValue = false;
		fogStart = 10.0f;
		fogEnd = 35.0f;
		fogDensity = 0.04f;
		glUniform1i(fog_option, currentFogMode);
		glUniform1i(fog_debug, fogDebugValue);
		glUniform1f(fog_start, fogStart);
		glUniform1f(fog_end, fogEnd);
		glUniform1f(fog_density, fogDensity);
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
	void setBuildingTextureScale(int tb);
	vector<glm::vec3> getCameraBuildingScales() const { return building_scales;}
	vector<int> getCameraBuildingTexture() const { return random_texture; }
	void generateNewBuildingSetting();
	void buildingRandomizer();
};
#endif