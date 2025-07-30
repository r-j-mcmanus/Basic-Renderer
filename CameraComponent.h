#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Component.h"


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

class CameraComponent : public Component {
public:
    CameraComponent(glm::vec3 front);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const;

    void setAspectRatio(const int width, const int height);

    void setTarget(const glm::vec3& target);

    void setTarget(const float x, const float y, const float z);

    void setAspectRatio(float newAspectRatio);

    void onBuild(SceneNode& node);

    glm::vec3 getForward() const { return front; }
    glm::vec3 getUp() const { return up; }

    void updateCameraVectors(glm::vec3 rotation);

public:
    // camera Attributes
    glm::vec3 front;
    glm::vec3 up;
    glm::mat4 rotationMatrix;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;


private:
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    glm::mat4 viewMatrix;

};
