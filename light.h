#pragma once

#include <glm/glm.hpp>

struct Light {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
};