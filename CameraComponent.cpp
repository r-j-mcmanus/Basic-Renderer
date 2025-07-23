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
#include "SceneNode.h"

#include "helper.h"


CameraComponent::CameraComponent(glm::vec3 front): 
    up(CameraDefaultValues::UP),
    movementSpeed(CameraDefaultValues::SPEED),
    mouseSensitivity(CameraDefaultValues::SENSITIVITY), 
    zoom(CameraDefaultValues::ZOOM)
{
    this->front = glm::normalize(front);
}

void CameraComponent::updateCameraVectors(glm::vec3 rotation)
{
    // printVec3(rotation, "rotation");

    rotationMatrix = (
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * // Rotation around Z-axis (pitch)
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * // Rotation around Y-axis (yaw)
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) // Rotation around X-axis (roll)
        );

    front = glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void CameraComponent::onBuild(SceneNode& node) {
    parent = &node;
}

glm::mat4 CameraComponent::getViewMatrix() const
{
    /*
    Uses glm::lookAt(eye, center, up) to create a view matrix.
    eye = camera position (position).
    center = point the camera is looking at (position + front).
    up = up direction for the camera (up).
    */

    glm::vec3 position = parent->getGlobalPosition();
    return glm::lookAt(position, position + front, up);
}

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
