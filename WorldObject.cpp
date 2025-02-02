
#include "WorldObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective


WorldObject::WorldObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int modelId, unsigned int shaderId)
    : position(position), rotation(rotation), scale(scale), renderingComponent({modelId, shaderId})
{
    updateModelMatrix();
}

void WorldObject::setPosition(const glm::vec3& position) {
    this->position = position;
    updateModelMatrix();
}

void WorldObject::setRotation(const glm::vec3& rotation) {
    this->rotation = rotation;
    updateModelMatrix();
}

void WorldObject::setScale(const glm::vec3& scale) {
    this->scale = scale;
    updateModelMatrix();
}

glm::mat4 WorldObject::getModelMatrix() const {
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
    position += dt * velocity;
}
