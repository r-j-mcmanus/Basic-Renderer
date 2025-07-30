#pragma once

#include <glm/glm.hpp>

#include "Controls.h"
#include "KeyTracker.h"
#include "MouseTracker.h"

#include "CameraComponent.h"

class FirstCameraControls : public Controls {
public:
	void fixedUpdateKey(float dt) {
        const glm::vec3 forward = parent->getComponent<CameraComponent>()->getForward();  // based on rotation
        const glm::vec3 up = parent->getComponent<CameraComponent>()->getUp();
        const glm::vec3 right = glm::cross(forward, up);
        const float moveSpeed = parent->getComponent<CameraComponent>()->movementSpeed;
        if (keyTracker->isKeyDown(GLFW_KEY_W)) parent->translate(forward * moveSpeed * dt);
        if (keyTracker->isKeyDown(GLFW_KEY_S)) parent->translate(-forward * moveSpeed * dt);
        if (keyTracker->isKeyDown(GLFW_KEY_A)) parent->translate(-right * moveSpeed * dt);
        if (keyTracker->isKeyDown(GLFW_KEY_D)) parent->translate(right * moveSpeed * dt);
        if (keyTracker->isKeyDown(GLFW_KEY_Q)) parent->translate(-up * moveSpeed * dt);
        if (keyTracker->isKeyDown(GLFW_KEY_E)) parent->translate(up * moveSpeed * dt);
    };

    void fixedUpdateMouse(float dt) {
        const glm::vec2 delta = mouseTracker->getDelta();

        if (delta.x != 0 or delta.y != 0)
        {
            //printVec2(delta, "mouse delta");
            const float sensitivity = parent->getComponent<CameraComponent>()->mouseSensitivity;
            parent->rotate(delta.x * sensitivity, delta.y * sensitivity);
            parent->getComponent<CameraComponent>()->updateCameraVectors(parent->getRotation());
        }
    };
};