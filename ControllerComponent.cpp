#include "ControllerComponent.h"
#include "CameraComponent.h"
#include "SceneNode.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "helper.h"

ControllerComponent::ControllerComponent(
    std::shared_ptr<KeyTracker>& keyTracker,
    std::shared_ptr<MouseTracker>& mouseTracker
): keyTracker(keyTracker), mouseTracker(mouseTracker){

}


void ControllerComponent::fixedUpdate(float dt) 
{
    fixedUpdateKey(dt);
    fixedUpdateMouse(dt);
}


void ControllerComponent::fixedUpdateKey(float dt)
{
    glm::vec3 forward = parent->getComponent<CameraComponent>()->getForward();  // based on rotation
    glm::vec3 up = parent->getComponent<CameraComponent>()->getUp();
    glm::vec3 right = glm::cross(forward, up);

    if (keyTracker->isKeyDown(GLFW_KEY_W)) parent->translate(forward * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_S)) parent->translate(-forward * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_A)) parent->translate(-right * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_D)) parent->translate(right * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_Q)) parent->translate(-up * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_E)) parent->translate(up * moveSpeed * dt);
}

void ControllerComponent::fixedUpdateMouse(float dt)
{
    glm::vec2 delta = mouseTracker->getDelta();
    
    if (delta.x != 0 or delta.y != 0)
    {
        //printVec2(delta, "mouse delta");
        parent->rotate(delta.x * sensitivity, delta.y * sensitivity);
        parent->getComponent<CameraComponent>()->updateCameraVectors(parent->getRotation());
    }
}