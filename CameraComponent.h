#pragma once

#include "Component.h"
#pragma once

#include <glm/glm.hpp>

#include <iostream>

#include "EventListenerInterface.h"


struct CameraDirectionContainer {
    bool left;
    bool right;
    bool forward;
    bool back;
};

namespace CameraDefaultValues {
    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
}

class CameraComponent : public EventListenerInterface, public Component {
public:
    CameraComponent(glm::vec3 up = CameraDefaultValues::UP, float yaw = CameraDefaultValues::YAW, float pitch = CameraDefaultValues::PITCH);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    // glm::mat4 getViewMatrix() const;

    void setAspectRatio(const int width, const int height);

    void setTarget(const glm::vec3& target);

    void setTarget(const float x, const float y, const float z);

    void setAspectRatio(float newAspectRatio);

    glm::mat4 getProjectionMatrix() const;

    void fixedUpdate(float dt);

public:
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


private:
    glm::vec3 viewDirection;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float lastX = 0;
    float lastY = 0;
    float deltaYaw = 0;
    float deltaPitch = 0;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    CameraDirectionContainer directionContainer = { false, false, false, false };

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    void onKeyEvent(int key, int scancode, int action, int mods) override;
    void onMouseMovement(int xpos, int ypos) override;
};
