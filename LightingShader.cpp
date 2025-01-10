/*

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightingShader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

void LightingShader::SetUniforms(const glm::mat4 Model, const Camera camera, const Material material, const Light light) {
	this->Bind();
	this->setVec3("u_material.ambient", material.ambient);
	this->setVec3("u_material.diffuse", material.diffuse);
	this->setVec3("u_material.specular", material.specular);
	this->setFloat("u_material.shininess", material.shininess);

	this->setVec3("u_light.ambient", light.ambient);
	this->setVec3("u_light.diffuse", light.diffuse);
	this->setVec3("u_light.specular", light.specular);

	this->setVec3("u_lightPos", light.lightPos);

	this->setVec3("u_viewPos", camera.getPosition());
	this->setMat4("u_view", camera.getViewMatrix());
	this->setMat4("u_projection", camera.getProjectionMatrix());

	this->setMat4("u_model", Model);
}

*/