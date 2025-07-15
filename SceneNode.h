#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include <glm/vec3.hpp>

//#include "SceneNodeBuilder.h"
#include "Component.h"

class SceneNodeBuilder;

class SceneNode
{
public:
	void earlyUpdate() {
		for (auto& child: children)
		{
			child->earlyUpdate();
		}
	};
	void update() {
		for (auto& child : children)
		{
			child->update();
		}
	};
	void fixedUpdate(float fixedDelta) {
		for (auto& child : children)
		{
			child->fixedUpdate(fixedDelta);
		}
	};
	void lateUpdate() {
		for (auto& child : children)
		{
			child->lateUpdate();
		}
	};
	void render() {
		for (auto& child : children)
		{
			child->render();
		}
	};

	void add_child(std::unique_ptr<SceneNode> child) {
		children.push_back(std::move(child));
	}

    //
    void setTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) {
        position = pos;
        rotation = rot;
        scale = scl;
    }

	template<typename T>
	std::shared_ptr<T> getComponent() {
		for (auto& c : components) {
			if (auto comp = std::dynamic_pointer_cast<T>(c.second)) // will cast to null if ptr does not exist
			{
				return comp;
			}
		}
		return nullptr;
	}

	template<typename Func>
	void traverse(Func func) {
		func(this);
		for (auto& child : children) {
			child->traverse(func);
		}
	}

public:
	bool isActive = true;

protected:
	void addComponent(int typeBitmask, std::shared_ptr<Component> component) {
		bitmask |= typeBitmask;
		components[typeBitmask] = component;
	}

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	int bitmask = 0;
	std::unordered_map<int, std::shared_ptr<Component>> components = std::unordered_map<int, std::shared_ptr<Component>>();

	friend class SceneNodeBuilder;
};