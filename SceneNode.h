#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective


//#include "SceneNodeBuilder.h"
#include "helper.h"
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
		for (auto& component : components)
		{
			component->fixedUpdate(fixedDelta);
		}
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
        rotationDegrees = rot;
        scale = scl;
    }
	
	void translate(const glm::vec3& delta) {
		MarkDirty();
		position += delta;
		printVec3(position, "position");
	}
	
	void rotate(const float yawDelta, float pitchDelta) {
		MarkDirty();
		// Update rotation
		rotationDegrees.y += yawDelta;   // Yaw (left/right)
		rotationDegrees.x += pitchDelta; // Pitch (up/down)

		// Clamp pitch to avoid flipping
		const float pitchLimit = 89.0f;
		if (rotationDegrees.x > pitchLimit) rotationDegrees.x = pitchLimit;
		if (rotationDegrees.x < -pitchLimit) rotationDegrees.x = -pitchLimit;

		// Keep yaw within 0-360
		if (rotationDegrees.y > 360) rotationDegrees.y -= 360;
		if (rotationDegrees.y < 0) rotationDegrees.y += 360;


		//printVec3(rotationDegrees, "rotation");
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

	const glm::vec3 getGlobalPosition() {
		glm::vec3 globalPosition = glm::vec3(pairentNode->getGlobalMatrix()[3]) + position;
		// printVec3(globalPosition, "GlobalPosition");
		return globalPosition;
	}

	glm::vec3 getRotation() const { return rotationDegrees; }

private:
	void MarkDirty() {
		isDirty = true;
		for (auto& child : children) {
			child->MarkDirty();
		}
	}


protected:
	void addComponent(std::shared_ptr<Component> component) {
		// todo have a list of updatableComponents and allow for the component to add itself to that list if it wants
		components.push_back(component);
	}

	glm::mat4 TRSMatrix() const {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMatrix = (
			glm::rotate(glm::mat4(1.0f), glm::radians(rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f))
			);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

protected:
	SceneNode* pairentNode;

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotationDegrees = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isDirty = true;
	glm::mat4 localMatrix;  // local TRS transform
	glm::mat4 globalMatrix; // cached global transform
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	unsigned int bitmask = 0;
	std::vector<std::shared_ptr<Component>> components;

};