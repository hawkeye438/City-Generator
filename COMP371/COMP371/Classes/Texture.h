#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H


#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "..\soil\SOIL.h"

class Texture
{
public:
	Texture();
	~Texture();
	static void loadTexture(int value, const char* filename);
};
#endif