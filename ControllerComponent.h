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

private:
    float sensitivity = 0.1f;
    float moveSpeed = 1.0f;
    std::shared_ptr<KeyTracker> keyTracker;
    std::shared_ptr<MouseTracker> mouseTracker;
};
