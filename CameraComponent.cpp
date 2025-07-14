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
#include "CameraComponent.h"


CameraComponent::CameraComponent(glm::vec3 up, float yaw, float pitch)
    : Front(CameraDefaultValues::FRONT), MovementSpeed(CameraDefaultValues::SPEED),
    MouseSensitivity(CameraDefaultValues::SENSITIVITY), Zoom(CameraDefaultValues::ZOOM),
    WorldUp(up), Yaw(yaw), Pitch(pitch), Up(up)
{
    updateCameraVectors();
}

//glm::mat4 CameraComponent::getViewMatrix() const
//{
//    return glm::lookAt(Position, Position + Front, Up);
//}

void CameraComponent::setAspectRatio(const int width, const int height) {
    aspectRatio = (float)width / (float)height;
}

void CameraComponent::setTarget(const glm::vec3& target) {
    // todo
}

void CameraComponent::setTarget(const float x, const float y, const float z) {
    // todo
}

void CameraComponent::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
}

glm::mat4 CameraComponent::getProjectionMatrix() const {
    return projectionMatrix;
}

void CameraComponent::fixedUpdate(float dt) {
    float side = (directionContainer.right && !directionContainer.left) - (!directionContainer.right && directionContainer.left);
    float forward = (directionContainer.forward && !directionContainer.back) - (!directionContainer.forward && directionContainer.back);
    glm::vec3 direction = side * Right + forward * Front;

    if (forward != 0) {
        int a = 1;
    }

    Yaw += deltaYaw * MouseSensitivity;
    Pitch += deltaPitch * MouseSensitivity;

    // clamp amount above and bellow to prevent looking directly up or down
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    //std::cout << direction.x << ' ' << direction.y << ' ' << direction.z << ' ' << deltaYaw << ' ' << deltaPitch << std::endl;

    updateCameraVectors();
}

void CameraComponent::onMouseMovement(int xpos, int ypos) {

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    deltaYaw += xpos - lastX;
    deltaPitch += lastY - ypos;
    lastX = xpos;
    lastY = ypos;
}

void CameraComponent::onKeyEvent(int key, int scancode, int action, int mods) {
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
void CameraComponent::updateCameraVectors()
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
