#include <string>

#include "ModelManager.h"
#include "ShaderManager.h"
#include "UniformBufferManager.h"

// movement
// #include "CircularMovement.h"


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
	SceneNode* cameraNode = root.add_child(std::move(
		builder.setTransform(glm::vec3(5, 0, 0), glm::vec3(0), glm::vec3(0))
			.addComponent<CameraComponent>(glm::vec3(-1, 0, 0))
			.build()
	));
	activeCameraNode = cameraNode;
	///

	///
	Material floorMaterial = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	root.add_child(std::move(
		builder.setTransform(glm::vec3(0), glm::vec3(0), glm::vec3(10, 0, 10)) // we flaten it along the y axis and extend on the x and z
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
	Light light = {
		glm::vec4(0.2f, 0.2f, 0.2f, 0.0f), // light position
		glm::vec4(0.7f, 0.7f, 0.7f, 0.0f), // ambient white
		glm::vec4(0.2f, 0.2f, 0.2f, 0.0f), // diffuse white
		glm::vec4(1.0f, 0.0f, 1.0f, 0.0f), // specular
	};

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

