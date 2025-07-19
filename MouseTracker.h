#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

#include "EventListenerInterface.h"

class MouseTracker : public EventListenerInterface {
public:
    // Get the mouse movement delta
    double getDeltaX() const { return deltaX; }
    double getDeltaY() const { return deltaY; }
    glm::vec2 getDelta() const { return glm::vec2(deltaX, deltaY); }

    // Reset delta after processing (optional)
    void resetDelta() {
        deltaX = 0.0;
        deltaY = 0.0;
    }

private:
    double lastX = 0.0;
    double lastY = 0.0;
    double deltaX = 0.0;
    double deltaY = 0.0;
    bool firstMouse = true;

protected:
    void onMouseMovement(double xpos, double ypos) {
        std::cout << "processing mouse event\n";
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        deltaX = xpos - lastX;
        deltaY = ypos - lastY;

        lastX = xpos;
        lastY = ypos;
    }
};
