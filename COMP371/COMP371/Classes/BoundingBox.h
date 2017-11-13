#pragma once
#ifndef BOUNDINGBOX_H
#define BOUDNINGBOX_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>

class BoundingBox {
	glm::vec3 min_extent;//corner with smallest value
	glm::vec3 max_extent;//corner with biggest value

public:
	BoundingBox(glm::vec3 min, glm::vec3 max) : min_extent(min), max_extent(max) {};

	bool intersect(glm::vec3 point, float offset);

	glm::vec3 getMinExtent() const { return min_extent; }
	glm::vec3 getMaxExtent() const { return max_extent; }
};
#endif
