#include <string>

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
#include "CameraComponent.h"
#include "EventHandler.h"

#include "World.h"
#include "SceneNodeBuilder.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager,
	EventHandler& eventHandler
	)
{

	// shaders we will be using
	unsigned int shaderPhongId = shaderManager->loadShader(ShaderData::PhongShader);
	unsigned int shaderSolidColorId = shaderManager->loadShader(ShaderData::SolidColor);

	// models we will be using
	unsigned int modelIdFloor = modelManager->loadModel(std::string("Resources/Models/plain.gltf"));
	unsigned int modelIdMonkey = modelManager->loadModel(std::string("Resources/Models/monkey.gltf"));
	unsigned int cubeModelId = modelManager->loadModel(std::string("Resources/Models/cube.gltf"));

	// for making nodes to add to our scene graph
	SceneNodeBuilder builder(&eventHandler);

	///
	root.add_child(std::move(
		builder.setTransform(glm::vec3(0), glm::vec3(0), glm::vec3(10, 0, 10))
			.addComponent<CameraComponent>()
			.build()
	));
	///

	///
	Material floorMaterial = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	root.add_child(std::move(
		builder.setTransform(glm::vec3(0), glm::vec3(0), glm::vec3(10, 0, 10))
			.addComponent<RenderableComponent>(modelIdFloor, shaderPhongId)
			.build()
	));
	///

	///
	Material materialMonekey = {
		glm::vec3(10.0f / 255.0f, 100.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};

	root.add_child(std::move(
		builder.setTransform(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(1))
			.addComponent<RenderableComponent>(modelIdMonkey,shaderPhongId)
			.build()
	));


	///
	Light light = {};

	root.add_child(std::move(
		builder.setTransform(glm::vec3(2, 2, 0), glm::vec3(0), glm::vec3(0.1))
			.addComponent<RenderableComponent>(cubeModelId, shaderSolidColorId)
			.addComponent<LightComponent>(light)
			.build()
	));
	///
}

void World::fixedUpdate(double dt)
{
	root.fixedUpdate(dt);
}

void World::update()
{
	root.update();
	root.lateUpdate();
}

void World::earlyUpdate()
{
	root.earlyUpdate();
}

