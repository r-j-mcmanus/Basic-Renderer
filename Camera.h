#pragma once

#include <glm/glm.hpp>

#include <iostream>

#include "EventListenerInterface.h"


struct DirectionContainer {
    bool left;
    bool right;
    bool forward;
    bool back;
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera :  public EventListenerInterface {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const;

    /*
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void grocessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
    */

    void setAspectRatio(const int width, const int height);

    void setPosition(const glm::vec3& newPosition);

    void setTarget(const glm::vec3& target);

    void setAspectRatio(float newAspectRatio);

    glm::mat4 getProjectionMatrix() const;

    void update(float dt);

public:
    // camera Attributes
    glm::vec3 Position;
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

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    DirectionContainer directionContainer = {false, false, false, false};

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    void onKeyEvent(int key, int scancode, int action, int mods) override;
    void onMouseMovement(double xpos, double ypos) override;
};
