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
	const unsigned int shaderPhongMapId = shaderManager->loadShader(ShaderData::PhongShader);
	unsigned int shaderPhongId = shaderManager->getShader(shaderPhongMapId)->GetId(); // this should be moved into the shader manager...
	uniformBufferManager->bindBlockToShader(shaderPhongId, "Matrices", "ProjectionView");
	uniformBufferManager->bindBlockToShader(shaderPhongId, "Lights", "Lights");

	const unsigned int shaderSolidColorMapId = shaderManager->loadShader(ShaderData::SolidColor);
	unsigned int shaderSolidColorId = shaderManager->getShader(shaderSolidColorMapId)->GetId();
	uniformBufferManager->bindBlockToShader(shaderSolidColorId, "Matrices", "ProjectionView");

	const unsigned int shaderBasic3MapId = shaderManager->loadShader(ShaderData::Basic3);
	unsigned int shaderBasic3Id = shaderManager->getShader(shaderBasic3MapId)->GetId();
	uniformBufferManager->bindBlockToShader(shaderBasic3Id, "Matrices", "ProjectionView");

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
			.addComponent<RenderableComponent>(modelIdFloor, shaderBasic3MapId)
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
			.addComponent<RenderableComponent>(modelIdMonkey, shaderBasic3MapId)
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
			.addComponent<RenderableComponent>(cubeModelId, shaderBasic3MapId)
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

