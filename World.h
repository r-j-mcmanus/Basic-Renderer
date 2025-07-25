#pragma once

#include <vector>

#include "SceneNode.h"
#include "EventHandler.h"
#include "RenderingEngine.h"

class ModelManager;
class ShaderManager;
class UniformBufferManager;
class WorldObject;
class KeyTracker;
class MouseTracker;

class World
{
public:
	void buildWorld(
		unsigned int worldId,
		const std::shared_ptr<ModelManager>& modelManager,
		const std::shared_ptr<ShaderManager>& shaderManager,
		const std::shared_ptr<UniformBufferManager>& uniformBufferManager,
		std::shared_ptr<KeyTracker>& keyTracker,
		std::shared_ptr<MouseTracker>& mouseTracker
	);

	void fixedUpdate(double dt);
	void update();
	void earlyUpdate();


public:
	SceneNode root;
	SceneNode* activeCameraNode = nullptr; // this is a raw pointer to a node whos lifetime is controlled by the scene graph
};