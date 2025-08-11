#pragma once

#include <glm/glm.hpp>

struct AABB {
	glm::vec3 min; // smallest x,y,z
	glm::vec3 max; // largest x,y,z
};