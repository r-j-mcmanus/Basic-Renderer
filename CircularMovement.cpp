
#include "CircularMovement.h"
#include "WorldObject.h"
#include <iostream>

#include <glm/glm.hpp>


void print(glm::vec3 v) {
	std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;
}

/*

// this is the calc in full, but Rotation * inv_basis * r = (cos(theta), -sin(theta), 0) * |r|

void CircularMovement::update(WorldObject& obj, float deltaTime) {
	glm::vec3 r = obj.getPosition() - center;
	glm::vec3 r_hat = glm::normalize(obj.getPosition() - center);
	glm::vec3 normal = glm::cross(r_hat, tangent);
	glm::mat3 basis = glm::mat3(r_hat, tangent, normal);
	glm::mat3 inv_basis = glm::inverse(basis);
	float theta = deltaTime * angularVelocity;

	glm::mat3 Rotation(cos(theta), sin(theta), 0, -sin(theta), cos(theta), 0, 0, 0, 1);

	glm::vec3 updated_position = basis * Rotation * inv_basis * r + center;
	tangent = glm::normalize(glm::cross(normal, updated_position - center));

	print(basis * Rotation * inv_basis * r);

	obj.setPosition(updated_position);
}
*/

void CircularMovement::update(WorldObject& obj, float deltaTime) {
	glm::vec3 r = obj.getPosition() - center;
	glm::vec3 r_hat = glm::normalize(r);
	glm::vec3 normal = glm::cross(r_hat, tangent);
	glm::mat3 basis = glm::mat3(r_hat, tangent, normal);
	float theta = deltaTime * angularVelocity;	
	glm::vec3 updated_position = basis * glm::vec3(cos(theta), -sin(theta), 0) * glm::length(r) + center;
	tangent = glm::normalize(glm::cross(normal, updated_position - center));
	obj.setPosition(updated_position);
}