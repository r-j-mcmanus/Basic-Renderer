#pragma once
#include <vector>

#include "WorldObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"

class World
{
public:
	void buildWorld(unsigned int worldId, const std::shared_ptr<ModelManager>& modelManager, const std::shared_ptr<ShaderManager>& shaderManager);
	void update(double dt);
	// probably dont want this to be public
	std::vector<WorldObject> worldObjects;
};