#pragma once

#include <glm/glm.hpp>

// this needs to match the layout used in shaders
struct Light {
	glm::vec4 position;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
};