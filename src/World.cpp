#include <string>

// movement
// #include "CircularMovement.h"

// rendering
// #include "Basic5RenderingController.h"
// #include "Basic6RenderingController.h"

#include "ShaderData.h"

#include "LightComponent.h"
#include "AIComponent.h"
#include "RenderableComponent.h"
#include "TransformationComponent.h"
#include "CameraComponent.h"
#include "RibbonComponent.h"
#include "ControllerComponent.h"
#include "KeyTracker.h"
#include "MouseTracker.h"

#include "RandomWanderBehavior.h"

#include "World.h"
#include "SceneNodeBuilder.h"

#include "Material.h"

void World::buildWorld(
	unsigned int worldId, 
	RenderingEngine& renderingEngine,
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

	const unsigned int shaderBasic1Id = renderingEngine.shaderManager.loadShader(ShaderData::Basic1);
	renderingEngine.uniformBufferManager.bindBlockToShader(shaderBasic1Id, "Matrices", "ProjectionView");

	const unsigned int shaderRibbonId = renderingEngine.shaderManager.loadShader(ShaderData::Ribbon);
	renderingEngine.uniformBufferManager.bindBlockToShader(shaderRibbonId, "Matrices", "ProjectionView");
	renderingEngine.ribbonManager.shaderRibbonId = shaderRibbonId;
	////////

	// models we will be using
	unsigned int modelIdFloor = renderingEngine.modelManager.loadModel(std::string("Resources/Models/plain.gltf"));
	unsigned int modelIdMonkey = renderingEngine.modelManager.loadModel(std::string("Resources/Models/monkey.gltf"));
	unsigned int cubeModelId = renderingEngine.modelManager.loadModel(std::string("Resources/Models/cube.gltf"));
	unsigned int robotModelId = renderingEngine.modelManager.loadModel(std::string("Resources/Models/Robot.gltf"));

	// for making nodes to add to our scene graph
	SceneNodeBuilder builder;

	const glm::quat noRotation = glm::quat(1, 0, 0, 0);

	// Main character

	const glm::vec3 startingPos = glm::vec3(5, 2, 0);
	const glm::vec3 forward = glm::vec3(-1, 0, 0);
	const glm::vec3 up = glm::vec3(0, 1, 0);
	const glm::vec3 cameraOffset = -3.0f * forward;

	std::unique_ptr<SceneNode> character = builder.setTransform(startingPos, noRotation, glm::vec3(1.0f))
		.addComponent<ControllerComponent>(keyTracker, mouseTracker)
		.build();
	const glm::quat modelOrientation = glm::angleAxis(3.14159f / 2, glm::vec3(0, 1, 0)) * glm::angleAxis(3.14159f / 2, glm::vec3( 0,0,1 ));
	const glm::vec3 modelPosition = glm::vec3(0, 0, 0);
	std::unique_ptr<SceneNode> robot = builder.setTransform(modelPosition, modelOrientation, glm::vec3(0.1f))
		.addComponent<RenderableComponent>(robotModelId, shaderBasic1Id)
		//.addComponent<RibbonComponent>(renderingEngine.ribbonManager.getRibbonBuffer())
		.build();
	std::unique_ptr<SceneNode> camera = builder.setTransform(cameraOffset, noRotation, glm::vec3(0.0f))
		.addComponent<CameraComponent>(forward)
		.build();

	activeCameraNode = character->add_child(std::move(camera));
	character->add_child(std::move(robot));
	root.add_child(std::move(character));

	///
	Material floorMaterial = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	root.add_child(std::move(
		builder.setTransform(glm::vec3(0), noRotation, glm::vec3(10, 0, 10)) // we flaten it along the y axis and extend on the x and z
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
		builder.setTransform(glm::vec3(0, 1, 0), noRotation, glm::vec3(1))
			.addComponent<RenderableComponent>(modelIdMonkey, shaderBasic1Id, Materials::orange)
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
		builder.setTransform(glm::vec3(2, 2, 0), noRotation, glm::vec3(0.1f))
			.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id)
			.addComponent<LightComponent>(light)
			.addComponent<AIComponent>(std::make_unique<RandomWanderBehavior>())
			.build()
	));
	///


	///
	std::unique_ptr<SceneNode> body = builder.setTransform(glm::vec3(-2, 2, 2), noRotation, glm::vec3(1))
		.addComponent<AIComponent>(std::make_unique<RandomWanderBehavior>())
		.build();

	std::unique_ptr<SceneNode> child = builder.setTransform(glm::vec3(0.1f, 0.1f, 0.1f), noRotation, glm::vec3(0.05f))
		.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id, Materials::green)
		.build();
	body->add_child(std::move(child));

	body->add_child(std::move(
		builder.setTransform(glm::vec3(0.1, -0.1, 0.1), noRotation, glm::vec3(0.05f))
		.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id, Materials::green)
		.build()
	));
	body->add_child(std::move(
		builder.setTransform(glm::vec3(-0.1, 0.1, 0.1), noRotation, glm::vec3(0.05f))
		.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id, Materials::green)
		.build()
	));
	body->add_child(std::move(
		builder.setTransform(glm::vec3(0.1, 0.1, -0.1), noRotation, glm::vec3(0.05f))
		.addComponent<RenderableComponent>(cubeModelId, shaderBasic1Id, Materials::green)
		.build()
	));

	root.add_child(std::move(body));
	///

	// Ribbon
	
	root.add_child(std::move(
		builder.setTransform(glm::vec3(3, 2, 3), noRotation, glm::vec3(1.0f))
		//.addComponent<RenderableComponent>(cubeModelId, shaderRibbonId)
		.addComponent<RibbonComponent>(renderingEngine.ribbonManager.getRibbonBuffer())
		.addComponent<AIComponent>(std::make_unique<RandomWanderBehavior>(5.0f, 5.0f))
		.build()
	));
	
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

