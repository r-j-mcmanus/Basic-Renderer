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
    void setTransform(const glm::vec3& pos, const glm::vec3& rotDeg, const glm::vec3& scl) {
        position = pos;
        eulerAnglesDegrees = rotDeg;
        scale = scl;
    }
	
	void translate(const glm::vec3& delta) {
		MarkDirty();
		position += delta;
	}
	
	void rotate(const float yawDeltaDeg, const float pitchDeltaDeg, const float rollDeltaDeg) {
		MarkDirty();
		// Update rotation
		eulerAnglesDegrees.y += yawDeltaDeg;   // Yaw (left/right)
		eulerAnglesDegrees.x += pitchDeltaDeg; // Pitch (up/down)
		eulerAnglesDegrees.z += rollDeltaDeg; // Pitch (up/down)

		// Clamp pitch to avoid flipping
		const float pitchLimit = 89.0f;
		if (eulerAnglesDegrees.x > pitchLimit) eulerAnglesDegrees.x = pitchLimit;
		if (eulerAnglesDegrees.x < -pitchLimit) eulerAnglesDegrees.x = -pitchLimit;

		// Keep yaw within 0-360
		if (eulerAnglesDegrees.y > 360) eulerAnglesDegrees.y -= 360;
		if (eulerAnglesDegrees.y < 0) eulerAnglesDegrees.y += 360;

		// Keep yaw within 0-360
		if (eulerAnglesDegrees.z > 360) eulerAnglesDegrees.z -= 360;
		if (eulerAnglesDegrees.z < 0) eulerAnglesDegrees.z += 360;

		printVec3(eulerAnglesDegrees, "rotation");
	}


	void rotateYaw(const float yawDelta) {
		MarkDirty();
		// Update rotation
		eulerAnglesDegrees.y += yawDelta * 50;

		// Keep within 0-360
		if (eulerAnglesDegrees.y > 360) eulerAnglesDegrees.y -= 360;
		if (eulerAnglesDegrees.y < 0) eulerAnglesDegrees.y += 360;

		printVec3(eulerAnglesDegrees, "rotationDegrees");
	}

	void rotatePitch(const float pitchDelta) {
		MarkDirty();
		// Update rotation
		eulerAnglesDegrees.x += pitchDelta * 50;

		// Keep within 0-360
		if (eulerAnglesDegrees.x > 360) eulerAnglesDegrees.x -= 360;
		if (eulerAnglesDegrees.x < 0) eulerAnglesDegrees.x += 360;

		printVec3(eulerAnglesDegrees, "rotationDegrees");
	}

	void rotateRoll(const float rollDelta) {
		MarkDirty();
		// Update rotation
		eulerAnglesDegrees.z += rollDelta * 50;

		// Keep within 0-360
		if (eulerAnglesDegrees.z > 360) eulerAnglesDegrees.z -= 360;
		if (eulerAnglesDegrees.z < 0) eulerAnglesDegrees.z += 360;

		printVec3(eulerAnglesDegrees, "rotationDegrees");
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
		if (pairentNode)
		{
			glm::mat4 globalMatrix = pairentNode->getGlobalMatrix();
			glm::vec3 globalPosition = glm::vec3(pairentNode->getGlobalMatrix()[3]) + position;
			glm::vec3 globalAlt = glm::vec3(pairentNode->getGlobalMatrix() * glm::vec4(position, 1));
			//printMat4(globalMatrix, "globalMatrix");
			//printVec3(globalPosition, "globalPosition");
			//printVec3(globalAlt, "globalAlt");
			return globalAlt;
		}
		else
		{
			return position;
		}
	}

	glm::vec3 getRotation() const { return eulerAnglesDegrees; }

	glm::vec3 getPairentNodePosition() const { return pairentNode->getGlobalPosition(); }

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
			glm::rotate(glm::mat4(1.0f), glm::radians(eulerAnglesDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(eulerAnglesDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(eulerAnglesDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f))
			);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

protected:
	SceneNode* pairentNode;

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 eulerAnglesDegrees = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isDirty = true;
	glm::mat4 localMatrix;  // local TRS transform
	glm::mat4 globalMatrix; // cached global transform
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	unsigned int bitmask = 0;
	std::vector<std::shared_ptr<Component>> components;

};