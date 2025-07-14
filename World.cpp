#include <string>

#include "WorldObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "UniformBufferManager.h"

// movement
#include "CircularMovement.h"


// rendering
// #include "Basic5RenderingController.h"
// #include "Basic6RenderingController.h"

#include "ShaderData.h"

#include "LightComponent.h"
#include "RenderableComponent.h"
#include "TransformationComponent.h"

#include "World.h"
#include "SceneNodeBuilder.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager)
{

	// shaders we will be using
	unsigned int shaderPhongId = shaderManager->loadShader(ShaderData::PhongShader);
	unsigned int shaderSolidColorId = shaderManager->loadShader(ShaderData::SolidColor);


	// for making nodes to add to our scene graph
	SceneNodeBuilder builder;


	///
	const std::string floorFilePath = "Resources/Models/plain.gltf";
	unsigned int modelIdFloor = modelManager->loadModel(floorFilePath);
	Material floorMaterial = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	root.add_child(std::move(
		builder.setTransform(glm::vec3(0), glm::vec3(0), glm::vec3(10, 0, 10))
			.addComponent<RenderableComponent>()
			.build()
	));
	///

	///
	const std::string monkeyFilePath = "Resources/Models/monkey.gltf";
	unsigned int modelIdMonkey = modelManager->loadModel(monkeyFilePath);
	Material materialMonekey = {
		glm::vec3(10.0f / 255.0f, 100.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};

	root.add_child(std::move(
		builder.setTransform(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(1))
			.addComponent<RenderableComponent>() // std::make_unique<PhongRenderingData>(modelIdMonkey, shaderPhongId, materialMonekey), std::make_unique<Basic5RenderingController>());
			.build()
	));


	///
	const std::string cubeFilePath = "Resources/Models/cube.gltf";
	unsigned int cubeModelId = modelManager->loadModel(cubeFilePath);
	Light light = {};

	root.add_child(std::move(
		builder.setTransform(glm::vec3(2, 2, 0), glm::vec3(0), glm::vec3(0.1))
			.addComponent<RenderableComponent>() //  std::make_unique<SolidRenderingData>(cubeModelId, shaderSolidColorId, glm::vec3(0, 0, 0))), std::make_unique<SolidColorRenderingController>())
			.addComponent<LightComponent>(light)
			.build()
	));
	///
}

void World::update(double dt)
{
	// id will eventually indicate which world to build?
	for (auto& wo : worldObjects) {
		wo.update(dt);
	}
}
