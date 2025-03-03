#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <glm/vec3.hpp>

#include "SceneNodeBuilder.h"
#include "Component.h"

enum NodeType {
	NONE = 0,
	RENDERABLE = 1 << 0,
	LIGHT = 1 << 1,
	CAMERA = 1 << 2,
	PHYSICS = 1 << 3,
};

// Base class for all components

class SceneNode
{
public:
	SceneNode(int nodeType = NodeType::NONE);
	SceneNode(int nodeType = NodeType::NONE, std::unordered_map<int, std::shared_ptr<Component>> components);

	void update();
	void add_child(std::unique_ptr<SceneNode> child);
	bool hasType(int type) const;

public:
	bool isActive = true;

protected:
	void addComponent(int type, std::shared_ptr<Component> component) {
		components[type] = component;
	}

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::vector<std::unique_ptr<SceneNode>> children;
	int bitmask;
	std::unordered_map<int, std::shared_ptr<Component>> components;

	friend class NodeBuilder;
};