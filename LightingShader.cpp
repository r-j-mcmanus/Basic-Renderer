#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightingShader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

void LightingShader::SetUniforms(const glm::mat4 Model, const Camera camera, const Material material, const Light light) {
	this->Bind();
	this->SetUniform3f("u_material.ambient", material.ambient);
	this->SetUniform3f("u_material.diffuse", material.diffuse);
	this->SetUniform3f("u_material.specular", material.specular);
	this->setUniformFloat("u_material.shininess", material.shininess);

	this->SetUniform3f("u_light.ambient", light.ambient);
	this->SetUniform3f("u_light.diffuse", light.diffuse);
	this->SetUniform3f("u_light.specular", light.specular);

	this->SetUniform3f("u_lightPos", light.lightPos);

	this->SetUniform3f("u_viewPos", camera.getPosition());
	this->setUniformMat4("u_view", camera.getViewMatrix());
	this->setUniformMat4("u_projection", camera.getProjectionMatrix());

	this->setUniformMat4("u_model", Model);
}