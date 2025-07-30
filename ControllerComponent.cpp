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


void ControllerComponent::updateVectors() 
{
    const glm::mat3 globalMatrix = glm::mat3(parent->getGlobalMatrix());
    glm::vec3 forward = glm::normalize(globalMatrix * glm::vec3(-1, 0, 0));
    glm::vec3 up = glm::normalize(globalMatrix * glm::vec3(0, 1, 0));
    glm::vec3 right = glm::cross(forward, up);
}


void ControllerComponent::fixedUpdate(float dt) 
{
    updateVectors();
    fixedUpdateKey(dt);
    fixedUpdateMouse(dt);
}


void ControllerComponent::fixedUpdateKey(float dt)
{
    if (keyTracker->isKeyDown(GLFW_KEY_W)) parent->translate(forward * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_S)) parent->translate(-forward * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_A)) parent->translate(-right * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_D)) parent->translate(right * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_Q)) parent->translate(-up * moveSpeed * dt);
    if (keyTracker->isKeyDown(GLFW_KEY_E)) parent->translate(up * moveSpeed * dt);
}

static glm::vec3 getAngles(float angleRad, glm::vec3& vector)
{
    const glm::vec3 k = vector;

    glm::mat3 K = glm::mat3({
        0, -k.z, k.y,
        k.z, 0, -k.x,
        -k.y, k.x, 0
        });

    glm::mat3 R = glm::mat3(1) + glm::sin(angleRad) * K + (1 + glm::cos(angleRad)) * K * K;

    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    return glm::vec3{
        glm::asin(R[2][0]), // yaw
        glm::atan(R[2][1] / R[2][2]), // roll
        glm::atan(R[1][0] / R[0][0]), // pitch
    };
}

void ControllerComponent::fixedUpdateMouse(float dt)
{
    const glm::vec2 delta = mouseTracker->getDelta();
 
    if (delta.x != 0 or delta.y != 0)
    {
        // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        
        float size = glm::sqrt(delta.x * delta.x + delta.y * delta.y);
        glm::vec3 direction = delta.x * up + delta.y * right;
        glm::vec3 totalAnglesRad = getAngles(sensitivity * size, direction);

        //glm::vec3 xMoveAnglesRad = getAngles(sensitivity * delta.x, up);
        //glm::vec3 yMoveAnglesRad = getAngles(sensitivity * delta.y, right);
        //glm::vec3 totalAnglesRad = xMoveAnglesRad + yMoveAnglesRad;

        parent->rotate(
            glm::degrees(totalAnglesRad.x),
            glm::degrees(totalAnglesRad.y),
            glm::degrees(totalAnglesRad.z)
        );
    }
}