#pragma once

#include <vector>

#include "WorldObject.h"

class ModelManager;
class ShaderManager;
class UniformBufferManager;
class WorldObject;

class World
{
public:
	void buildWorld(
		unsigned int worldId, 
		const std::shared_ptr<ModelManager>& modelManager, 
		const std::shared_ptr<ShaderManager>& shaderManager, 
		const std::shared_ptr<UniformBufferManager>& uniformBufferManager
	);

	void update(double dt);

public:
	// probably dont want this to be public
	std::vector<WorldObject> worldObjects;
};