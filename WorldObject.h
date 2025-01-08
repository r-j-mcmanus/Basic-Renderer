#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective


struct RenderComponent {
    unsigned int modelID;
    unsigned int shaderID;
};

class WorldObject {
public:
    WorldObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int modelId, unsigned int shaderId);

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    glm::mat4 getModelMatrix() const;

public:
    // this should be done differently, maybe a manager that links a world object id to the relevant reasorces
    // particularly if the system gets more complex
    RenderComponent renderingComponent;

private:
    void updateModelMatrix() const;

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    mutable glm::mat4 modelMatrix;

};