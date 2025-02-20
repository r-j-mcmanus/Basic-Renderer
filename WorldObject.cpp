
#include "WorldObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective

#include "MovementController.h"
#include "RenderingController.h"
#include "Camera.h"


WorldObject::WorldObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int modelId, unsigned int shaderId)
    : position(position), rotation(rotation), scale(scale), renderingComponent({modelId, shaderId})
{
    updateModelMatrix();
}

void WorldObject::setPosition(const glm::vec3& position) {
    this->position = position;
}

void WorldObject::setRotation(const glm::vec3& rotation) {
    this->rotation = rotation;
}

void WorldObject::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

glm::mat4 WorldObject::getModelMatrix() {
    updateModelMatrix();
    return modelMatrix;
}

void WorldObject::updateModelMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = (
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
        );
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void WorldObject::update(float dt) {
    if (movementController) movementController->update(*this, dt);

}

void WorldObject::draw(std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager, std::shared_ptr<Camera> activeCamera) {
    if (renderingController) renderingController->draw(*this, shaderManager, modelManager, activeCamera);

}

void WorldObject::setMovementController(std::unique_ptr<MovementController> controller) {
    movementController = std::move(controller);
}

void WorldObject::setRendererController(std::unique_ptr<RenderingController> controller) {
    renderingController = std::move(controller);
}