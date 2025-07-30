#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Component.h"
#include "KeyTracker.h"
#include "MouseTracker.h"


class ControllerComponent : public Component {
public:
    ControllerComponent(
        std::shared_ptr<KeyTracker>& keyTracker,
        std::shared_ptr<MouseTracker>& mouseTracker
    );

    void fixedUpdate(float dt);

private:
    void fixedUpdateKey(float dt);
    void fixedUpdateMouse(float dt);
    void updateVectors();

private:
    glm::vec3 forward = glm::vec3(1, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);;
    glm::vec3 right = glm::vec3(0, 0, 1);;

    float movementSpeed = 5.0f;
    float sensitivity = 0.05f;
    float mouseSensitivity = 1.0f;
    float moveSpeed = 1.0f;
    std::shared_ptr<KeyTracker> keyTracker;
    std::shared_ptr<MouseTracker> mouseTracker;
};
