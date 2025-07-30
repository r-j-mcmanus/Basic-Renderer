#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ControllerComponent.h"
#include "CameraComponent.h"
#include "SceneNode.h"
#include "helper.h"


ControllerComponent::ControllerComponent(
    std::shared_ptr<KeyTracker>& keyTracker,
    std::shared_ptr<MouseTracker>& mouseTracker
): keyTracker(keyTracker), mouseTracker(mouseTracker){

}


void ControllerComponent::updateVectors() 
{
    const glm::mat3 globalMatrix = glm::mat3(parent->getGlobalMatrix());
    forward = glm::normalize(globalMatrix * glm::vec3(-1, 0, 0));
    up = glm::vec3(0, 1, 0);
    right = glm::cross(forward, up);
}


void ControllerComponent::fixedUpdate(float dt) 
{
    updateVectors();
    fixedUpdateKey(dt);
    fixedUpdateMouse(dt);
}


void ControllerComponent::fixedUpdateKey(float dt)
{
    float speed = 0;
    if (
        (keyTracker->isKeyDown(GLFW_KEY_W) | keyTracker->isKeyDown(GLFW_KEY_S)) &
        (keyTracker->isKeyDown(GLFW_KEY_A) | keyTracker->isKeyDown(GLFW_KEY_D))
    ) {
        speed = glm::sqrt(movementSpeed);
    }
    else
    {
        speed = movementSpeed;
    }
    if (keyTracker->isKeyDown(GLFW_KEY_W)) parent->translate(forward * speed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_S)) parent->translate(-forward * speed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_A)) parent->translate(-right * speed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_D)) parent->translate(right * speed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_Q)) parent->translate(up * speed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_E)) parent->translate(-up * speed * dt);
}


void ControllerComponent::fixedUpdateMouse(float dt)
{
    const glm::vec2 delta = mouseTracker->getDelta();
 
    if (delta.x != 0 or delta.y != 0)
    {        
        float angleRad = - sensitivity * delta.y;

        // clamp pitch - tracked seperatly
        pitch = pitch + angleRad;
        if (pitch > pitchClamp) {
            float detaAngle = pitch - pitchClamp;
            pitch = pitchClamp;
            angleRad = angleRad - detaAngle;
        }
        if (pitch < -pitchClamp) {
            float detaAngle = -pitchClamp - pitch;
            pitch = -pitchClamp;
            angleRad = angleRad + detaAngle;
        }

        glm::vec3 direction = right;
        const glm::quat rotationRightRad = glm::angleAxis(angleRad, direction);

        angleRad = - sensitivity * delta.x;
        direction = up;
        const glm::quat rotationUpRad = glm::angleAxis(angleRad, direction);

        parent->rotate(rotationUpRad);
        parent->rotate(rotationRightRad);

        updateVectors();
    }
}