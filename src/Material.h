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

namespace Materials {

	const Material orange = {
		glm::vec3(1, 127.f / 255.f, 39.f / 255.f),
		glm::vec3(0.5f),
		glm::vec3(0.5f),
		1.0f
	};

	const Material red = {
		glm::vec3(237.f / 255.f, 28.f / 255.f, 36.f / 255.f),
		glm::vec3(0.5f),
		glm::vec3(0.5f),
		1.0f
	};

	const Material green = {
		glm::vec3(34 / 255.f, 177.f / 255.f, 76.f / 255.f),
		glm::vec3(0.5f),
		glm::vec3(0.5f),
		1.0f
	};

}