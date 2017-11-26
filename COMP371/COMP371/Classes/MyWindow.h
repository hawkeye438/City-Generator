#pragma once
#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "Camera.h"

class MyWindow
{
	Camera* camera;
public:
	MyWindow(Camera* c):camera(c) {}

	void cameraControls(GLFWwindow* window, int key, int scancode, int action, int mode) { camera->cameraKeys(window, key, scancode, action, mode); }
	void cameraMouseControls(GLFWwindow* window, double xpos, double ypos) { camera->cameraMouse(window, xpos, ypos); }
	void cameraMouseButtonsControls(GLFWwindow* window, int button, int action, int mods) { camera->cameraMouseButtons(window, button, action, mods); }
	void cameraResize(GLFWwindow* window, int width, int height) { camera->cameraResize(window, width, height); }
};
#endif