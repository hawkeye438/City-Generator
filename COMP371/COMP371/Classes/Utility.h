#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>

class Utility {


public:
	static void setMinMaxVert(float &min_x, float &max_x, float &min_y, float &max_y, float &min_z, float &max_z, std::vector<glm::vec3> vertices) {
		min_x = max_x = vertices[0].x;
		min_y = max_y = vertices[0].y;
		min_z = max_z = vertices[0].z;
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices[i].x < min_x) min_x = vertices[i].x;
			if (vertices[i].x > max_x) max_x = vertices[i].x;
			if (vertices[i].y < min_y) min_y = vertices[i].y;
			if (vertices[i].y > max_y) max_y = vertices[i].y;
			if (vertices[i].z < min_z) min_z = vertices[i].z;
			if (vertices[i].z > max_z) max_z = vertices[i].z;
		}
	}
};
#endif