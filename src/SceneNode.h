#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
    void setTransform(const glm::vec3& pos, const glm::quat& rotRad, const glm::vec3& scl) {
        position = pos;
		rotationRad = rotRad;
        scale = scl;
    }
	
	void translate(const glm::vec3& delta) {
		MarkDirty();
		position += delta;
	}
	
	void rotate(glm::quat rotation) {
		MarkDirty();
		// Update rotation
		rotationRad = rotation * rotationRad;
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

	// returns the Pairents global matrix, or an identy matrix
	glm::mat4 getPairentGlobalMatrix() {
		if (pairentNode) {
			return pairentNode->getGlobalMatrix();
		}
		else {
			return glm::mat4();
		}
	}

	const glm::vec3 getGlobalPosition() {
		if (pairentNode)
		{
			glm::vec3 globalPosition = glm::vec3(getGlobalMatrix()[3]);
			return globalPosition;
		}
		else
		{
			return position;
		}
	}

	glm::quat getRotation() const { return rotationRad; }
	void setRotation(glm::quat rotRad) { rotationRad = rotRad; }

	glm::vec3 getPosition() const { return position; };

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
		const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		const glm::mat4 rotationMatrix = glm::toMat4(rotationRad);
		const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

protected:
	SceneNode* pairentNode = nullptr;

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::quat rotationRad = glm::quat(1,0,0,0);
	// glm::vec3 eulerAnglesDegrees = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isDirty = true;
	glm::mat4 localMatrix;  // local TRS transform
	glm::mat4 globalMatrix; // cached global transform
	std::vector<std::unique_ptr<SceneNode>> children = std::vector<std::unique_ptr<SceneNode>>();
	unsigned int bitmask = 0;
	std::vector<std::shared_ptr<Component>> components;

};