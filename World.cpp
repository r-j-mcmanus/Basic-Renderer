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

	// const std::string shaderFilePath = "Resources/Shaders/mvp_uniform_buffer.shader";
	const std::string shaderFilePath = "Resources/Shaders/basic_3.shader";
	const std::string shaderName = "basic";
	unsigned int shaderId = shaderManager->loadShader(shaderName, shaderFilePath);




	///
	const std::string floorFilePath = "Resources/Models/plain.gltf";
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(10,0,10);
	unsigned int modelIdFloor = modelManager->loadModel(floorFilePath);
	WorldObject plain(position, rotation, scale, modelIdFloor, shaderId);
	worldObjects.push_back(plain);
	///

		//unsigned int modelId = 0;
	///
	const std::string monkeyFilePath = "Resources/Models/monkey.gltf";
	glm::vec3 position2 = glm::vec3(0, 1, 0);
	glm::vec3 rotation2 = glm::vec3(0);
	glm::vec3 scale2 = glm::vec3(1);
	unsigned int modelIdMonkey = modelManager->loadModel(monkeyFilePath);
	WorldObject monkey(position2, rotation2, scale2, modelIdMonkey, shaderId);
	worldObjects.push_back(monkey);
	///
}

void World::update(double dt)
{
	// id will eventually indicate which world to build?

	for (auto wo : worldObjects) {
		wo.update(dt);
	}

}
