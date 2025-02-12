#include <string>

#include "WorldObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "UniformBufferManager.h"
#include "CircularMovement.h"

#include "World.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager)
{
	// id will eventually indicate which world to build and the data for it to be stored in a file?

	// const std::string shaderFilePath = "Resources/Shaders/mvp_uniform_buffer.shader";
	const std::string shaderFilePath = "Resources/Shaders/basic_5.shader";
	const std::string shaderName = "basic";
	unsigned int shaderId = shaderManager->loadShader(shaderName, shaderFilePath);


	///
	const std::string floorFilePath = "Resources/Models/plain.gltf";
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(10,0,10);
	unsigned int modelIdFloor = modelManager->loadModel(floorFilePath);
	WorldObject plain(position, rotation, scale, modelIdFloor, shaderId);
	plain.material = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	worldObjects.push_back(std::move(plain));
	///

	///
	const std::string monkeyFilePath = "Resources/Models/monkey.gltf";
	glm::vec3 position2 = glm::vec3(0, 1, 0);
	glm::vec3 rotation2 = glm::vec3(0);
	glm::vec3 scale2 = glm::vec3(1);
	unsigned int modelIdMonkey = modelManager->loadModel(monkeyFilePath);
	WorldObject monkey(position2, rotation2, scale2, modelIdMonkey, shaderId);
	monkey.material = {
		glm::vec3(10.0f / 255.0f, 100.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	worldObjects.push_back(std::move(monkey));
	///

	///
	const std::string shaderFilePath2= "Resources/Shaders/basic_3.shader";
	const std::string shaderName2 = "solid_color";
	unsigned int shaderId2 = shaderManager->loadShader(shaderName, shaderFilePath);


	const std::string cubeFilePath = "Resources/Models/cube.gltf";
	glm::vec3 position3 = glm::vec3(2, 2, 0);
	glm::vec3 rotation3 = glm::vec3(0);
	glm::vec3 scale3 = glm::vec3(0.1);
	unsigned int cubeIdMonkey = modelManager->loadModel(cubeFilePath);
	WorldObject cube(position3, rotation3, scale3, cubeIdMonkey, shaderId2);

	cube.setMovementController(std::make_unique<CircularMovement>(1, glm::vec3(0, 2, 0), glm::vec3(0, 0, 1)));

	worldObjects.push_back(std::move(cube));
	///
}

void World::update(double dt)
{
	// id will eventually indicate which world to build?
	for (auto& wo : worldObjects) {
		wo.update(dt);
	}
}
