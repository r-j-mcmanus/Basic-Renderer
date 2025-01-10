#include <string>

#include "WorldObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "UniformBufferManager.h"

#include "World.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager)
{
	// id will eventually indicate which world to build and the data for it to be stored in a file?

	const std::string modelFilePath = "Resources/Models/monkey.gltf";
	const std::string shaderFilePath = "Resources/Shaders/mvp_uniform_buffer.shader";
	const std::string shaderName = "basic";
	
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	unsigned int modelId = modelManager->loadModel(modelFilePath);
	unsigned int shaderId = shaderManager->loadShader(shaderName, shaderFilePath);

	WorldObject wo(position, rotation, scale, modelId, shaderId);
	worldObjects.push_back(wo);
}

void World::update(double dt)
{
	// id will eventually indicate which world to build?


}
