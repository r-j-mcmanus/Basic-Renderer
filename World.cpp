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
#include "ControllerComponent.h"
#include "KeyTracker.h"
#include "MouseTracker.h"

#include "World.h"
#include "SceneNodeBuilder.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager,
	std::shared_ptr<KeyTracker>& keyTracker,
	std::shared_ptr<MouseTracker>& mouseTracker
	)
{

	////////
	// shaders we will be using
	/* todo Abstract binding away */
	//const unsigned int shaderPhongId = shaderManager->loadShader(ShaderData::PhongShader);
	//uniformBufferManager->bindBlockToShader(shaderPhongId, "Matrices", "ProjectionView");
	//uniformBufferManager->bindBlockToShader(shaderPhongId, "Lights", "Lights");

	//const unsigned int shaderSolidColorId = shaderManager->loadShader(ShaderData::SolidColor);
	//uniformBufferManager->bindBlockToShader(shaderSolidColorId, "Matrices", "ProjectionView");

	//const unsigned int shaderBasic3Id = shaderManager->loadShader(ShaderData::Basic3);
	//uniformBufferManager->bindBlockToShader(shaderBasic3Id, "Matrices", "ProjectionView");

	const unsigned int shaderBasic1Id = shaderManager->loadShader(ShaderData::Basic1);
	uniformBufferManager->bindBlockToShader(shaderBasic1Id, "Matrices", "ProjectionView");


	////////

	// models we will be using
	unsigned int modelIdFloor = modelManager->loadModel(std::string("Resources/Models/plain.gltf"));
	unsigned int modelIdMonkey = modelManager->loadModel(std::string("Resources/Models/monkey.gltf"));
	unsigned int cubeModelId = modelManager->loadModel(std::string("Resources/Models/cube.gltf"));

	// for making nodes to add to our scene graph
	SceneNodeBuilder builder;

	///
	SceneNode* cameraNode = root.add_child(std::move(
		builder.setTransform(glm::vec3(5, 0, 0), glm::vec3(0), glm::vec3(0))
			.addComponent<CameraComponent>(glm::vec3(-1, 0, 0))
			.addComponent<ControllerComponent>(keyTracker, mouseTracker)
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
			.addComponent<RenderableComponent>(modelIdFloor, shaderBasic1Id)
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
			.addComponent<RenderableComponent>(modelIdMonkey, shaderBasic1Id)
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
			.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id)
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

