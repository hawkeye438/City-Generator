#pragma once
#ifndef SHADOW_H
#define SHADOW_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include "../objloader.hpp"

class Shadow
{
public:

	void bindDepthMap(unsigned int &FBO, GLuint &depthMap);
	void renderDepthMap(unsigned int &FBO, GLuint &depthMapLoc);
};
#endif

