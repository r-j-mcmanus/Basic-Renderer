#include <string>

#include "WorldObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "UniformBufferManager.h"

// movement
#include "CircularMovement.h"


// rendering
#include "Basic5RenderingController.h"
#include "Basic6RenderingController.h"

#include "World.h"
#include "SceneNodeBuilder.h"

void World::buildWorld(
	unsigned int worldId, 
	const std::shared_ptr<ModelManager>& modelManager, 
	const std::shared_ptr<ShaderManager>& shaderManager, 
	const std::shared_ptr<UniformBufferManager>& uniformBufferManager)
{

	// shaders we will be using
	const std::string shaderFilePath = "Resources/Shaders/phong.shader";
	const std::string shaderName = "phong_shader";
	unsigned int shaderPhongId = shaderManager->loadShader(shaderName, shaderFilePath);

	const std::string shaderFilePath2 = "Resources/Shaders/solid_color.shader";
	const std::string shaderName2 = "solid_color";
	unsigned int shaderSolidColorId = shaderManager->loadShader(shaderName, shaderFilePath);



	// for making nodes to add to our scene graph
	NodeBuilder builder;


	///
	const std::string floorFilePath = "Resources/Models/plain.gltf";
	unsigned int modelIdFloor = modelManager->loadModel(floorFilePath);
	Material floorMaterial = {
		glm::vec3(100.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f),
		glm::vec3(210.0f / 255.0f, 105.0f / 255.0f, 34.0f / 255.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
	};
	builder.setTranformationData(glm::vec3(0), glm::vec3(0), glm::vec3(10,0,10))
		.addRenderableType(std::make_unique<PhongRenderingData>(modelIdFloor, shaderPhongId, floorMaterial), std::make_unique<PhongRenderingController>());
	root.add_child(builder.build());
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
	glm::vec3 position2 = glm::vec3(0, 1, 0);
	glm::vec3 rotation2 = glm::vec3(0);
	glm::vec3 scale2 = glm::vec3(1);

	builder.setTranformationData(position2, rotation2, scale2)
		   .addRenderableType(std::make_unique<PhongRenderingData>(modelIdMonkey, shaderPhongId, materialMonekey), std::make_unique<Basic5RenderingController>());
	root.add_child(builder.build());


	///


	const std::string cubeFilePath = "Resources/Models/cube.gltf";
	glm::vec3 position3 = glm::vec3(2, 2, 0);
	glm::vec3 rotation3 = glm::vec3(0);
	glm::vec3 scale3 = glm::vec3(0.1);
	unsigned int cubeModelId = modelManager->loadModel(cubeFilePath);
	Light light = {};

	builder.setTranformationData(position3, rotation3, scale3)
		.addRenderableType(std::make_unique<SolidColorRenderingData>(cubeModelId, shaderSolidColorId, glm::vec3(0, 0, 0)), std::make_unique<SolidColorRenderingController>())
		.addLightType(light);
	root.add_child(builder.build());

	//WorldObject cube(position3, rotation3, scale3, cubeIdMonkey, shaderId2);

	//cube.setMovementController(std::make_unique<CircularMovement>(1, glm::vec3(0, 2, 0), glm::vec3(0, 0, 1)));
	//cube.setRendererController(std::make_unique<Basic6RenderingController>());

	//worldObjects.push_back(std::move(cube));
	///
}

void World::update(double dt)
{
	// id will eventually indicate which world to build?
	for (auto& wo : worldObjects) {
		wo.update(dt);
	}
}
