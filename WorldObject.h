#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective

#include "Material.h"

#include "MovementController.h"

class MovementController;

struct RenderComponent {
    unsigned int modelID;
    unsigned int shaderID;
};



class WorldObject {
public:
    WorldObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int modelId, unsigned int shaderId);


    // Delete copy constructor (if needed)
    WorldObject(const WorldObject&) = delete;
    WorldObject& operator=(const WorldObject&) = delete;

    // Allow moving
    WorldObject(WorldObject&&) noexcept = default;
    WorldObject& operator=(WorldObject&&) noexcept = default;


    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const { return position; };
    void setRotation(const glm::vec3& rotation);
    glm::vec3 getRotation() const { return rotation; };
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const { return scale; };
    glm::mat4 getModelMatrix();

    void update(float dt);

    void setMovementController(std::unique_ptr<MovementController> controller);


    Material material;

public:
    // this should be done differently, maybe a manager that links a world object id to the relevant reasorces
    // particularly if the system gets more complex
    RenderComponent renderingComponent;

private:
    void updateModelMatrix();


private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec3 velocity;

    mutable glm::mat4 modelMatrix;
    std::unique_ptr<MovementController> movementController;
};