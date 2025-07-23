#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

const Material defaultMaterial = {
	glm::vec3(1),
	glm::vec3(1),
	glm::vec3(1),
	1.0f
};