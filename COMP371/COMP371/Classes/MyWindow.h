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
};
#endif