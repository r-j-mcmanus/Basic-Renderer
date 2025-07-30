#pragma once
#include <glm/glm.hpp>

#include "Controls.h"
#include "KeyTracker.h"
#include "MouseTracker.h"


class FirstCameraControls : public Controls {
public:
    void fixedUpdateKey(float dt) {
        if (keyTracker->isKeyDown(GLFW_KEY_Q)) {
            parent->rotateYaw(dt);
        }
        if (keyTracker->isKeyDown(GLFW_KEY_W)) {
            parent->rotatePitch(dt);
        }
        if (keyTracker->isKeyDown(GLFW_KEY_E)) {
            parent->rotateRoll(dt);
        }
    };

    void fixedUpdateMouse(float dt) {}
};