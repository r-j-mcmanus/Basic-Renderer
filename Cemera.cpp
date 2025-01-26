#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> // todo replace with quats

#include <iostream>

#include "Transformer.h"
#include "EventListenerInterface.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Position(position), WorldUp(up),
      Yaw(yaw), Pitch(pitch), Up(up)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}


void Camera::setAspectRatio(const int width, const int height) {
    aspectRatio = (float)width / (float)height;
}

void Camera::setPosition(const glm::vec3& newPosition) {
    Position = newPosition;
}

void Camera::setTarget(const glm::vec3& target) {
    viewDirection = glm::normalize(Position - target);
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::update(float dt) {
    float side = (directionContainer.right && !directionContainer.left) - (!directionContainer.right && directionContainer.left);
    float forward = (directionContainer.forward && !directionContainer.back) - (!directionContainer.forward && directionContainer.back);
    glm::vec3 direction = side * Right + forward * Front;

    if (forward != 0) {
        int a = 1;
    }

    Position += dt * MovementSpeed * direction;

    Yaw += deltaYaw;
    Pitch += deltaPitch;

    // clamp amount above and bellow to prevent looking directly up or down
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::onMouseMovement(double xpos, double ypos) {

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    deltaYaw += xoffset;
    deltaPitch += yoffset;
}


void Camera::onKeyEvent(int key, int scancode, int action, int mods) {
    bool isPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            directionContainer.forward = true;
        }
        else if (action == GLFW_RELEASE) {
            directionContainer.forward = false;
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            directionContainer.back = true;
        }
        else if (action == GLFW_RELEASE) {
            directionContainer.back = false;
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            directionContainer.left = true;
        }
        else if (action == GLFW_RELEASE) {
            directionContainer.left = false;
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            directionContainer.right = true;
        }
        else if (action == GLFW_RELEASE) {
            directionContainer.right = false;
        }
    }
}


// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
