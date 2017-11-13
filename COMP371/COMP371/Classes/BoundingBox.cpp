#include "BoundingBox.h"

bool BoundingBox::intersect(glm::vec3 point, float offset) {//offset is to account the object and how it renders in the world
	return (point.x >= (min_extent.x - offset) && point.x <= (max_extent.x + offset)) &&
		(point.y >= (min_extent.y - offset) && point.y <= (max_extent.y + offset)) &&
		(point.z >= (min_extent.z - offset) && point.z <= (max_extent.z + offset));
}