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
friend class SceneNodeBuilder;
friend class SceneNode;

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
		child->pairentNode = this;
		children.push_back(std::move(child));
		return rawPtr;
	}

    //
	// motion related functions
	//
    void setTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) {
        position = pos;
        rotation = rot;
        scale = scl;
    }
	
	void translate(const glm::vec3& delta) {
		MarkDirty();
		position += delta;
	}
	
	void rotate(const float yawDelta, float pitchDelta) {
		MarkDirty();
		// Update rotation
		rotation.y += yawDelta;   // Yaw (left/right)
		rotation.x += pitchDelta; // Pitch (up/down)

		// Clamp pitch to avoid flipping
		const float pitchLimit = 89.0f;
		if (rotation.x > pitchLimit) rotation.x = pitchLimit;
		if (rotation.x < -pitchLimit) rotation.x = -pitchLimit;

		// Keep yaw within 0-360
		if (rotation.y > 360) rotation.y -= 360;
		if (rotation.y < 0) rotation.y += 360;
	}
	//

	template<typename T>
	std::shared_ptr<T> getComponent() {
		for (auto& c : components) {
			if (auto comp = std::dynamic_pointer_cast<T>(c)) // will cast to null if ptr does not exist
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

	glm::mat4 getGlobalMatrix() {
		if (!isDirty) return globalMatrix;

		if (pairentNode) {
			globalMatrix = pairentNode->getGlobalMatrix() * TRSMatrix();
		}
		else {
			globalMatrix = TRSMatrix();
		}

		isDirty = false;
		return globalMatrix;

	}

	glm::vec3 getGlobalPosition() {
		return glm::vec3(pairentNode->getGlobalMatrix()[3]) + position;
	}

	glm::vec3 getRotation() const { return rotation; }

private:
	void MarkDirty() {
		isDirty = true;
		for (auto& child : children) {
			child->MarkDirty();
		}
	}


protected:
	void addComponent(std::shared_ptr<Component> component) {
		components.push_back(component);
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

protected:
	SceneNode* pairentNode;

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isDirty = true;
	glm::mat4 localMatrix;  // local TRS transform
	glm::mat4 globalMatrix; // cached global transform
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	unsigned int bitmask = 0;
	std::vector<std::shared_ptr<Component>> components;

};