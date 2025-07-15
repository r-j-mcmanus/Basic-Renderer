#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective


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

	SceneNode* add_child(std::unique_ptr<SceneNode> child) {
		SceneNode* rawPtr = child.get();
		children.push_back(std::move(child));
		return rawPtr;
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

	glm::mat4 GetGlobalMatrix() {
		if (!isDirty) return globalMatrix;

		if (pairentNode) {
			globalMatrix = pairentNode->GetGlobalMatrix() * TRSMatrix();
		}
		else {
			globalMatrix = TRSMatrix();
		}

		isDirty = false;
		return globalMatrix;

	}

public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isDirty = true;
	glm::mat4 localMatrix;  // local TRS transform
	glm::mat4 globalMatrix; // cached global transform

private:
	void MarkDirty() {
		isDirty = true;
		for (auto& child : children) {
			child->MarkDirty();
		}
	}


protected:
	void addComponent(int typeBitmask, std::shared_ptr<Component> component) {
		bitmask |= typeBitmask;
		components[typeBitmask] = component;
	}

	glm::mat4 TRSMatrix() const {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMatrix = (
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

private:
	SceneNode* pairentNode;
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	int bitmask = 0;
	std::unordered_map<int, std::shared_ptr<Component>> components = std::unordered_map<int, std::shared_ptr<Component>>();

	friend class SceneNodeBuilder;
	friend class SceneNode;
};