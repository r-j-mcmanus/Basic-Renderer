
#include "Basic5RenderingController.h"
#include "Shader.h"
#include "DrawableBuffer.h"
#include "Light.h"
#include "Camera.h"

void Basic5RenderingController::draw(WorldObject& obj, std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager, std::shared_ptr<Camera> activeCamera) {
    const unsigned int modelId = obj.renderingComponent.modelID;
    const unsigned int shaderId = obj.renderingComponent.shaderID;

    std::shared_ptr<DrawableBuffer> model = modelManager->getBuffer(modelId);
    std::shared_ptr<Shader> shader = shaderManager->getShader(shaderId);

    if (!model || !shader) {
        std::cout << "Model or shader ID does not correspond to a reseource" << std::endl;
        return;
    }

    shader->Bind();
    // currentShaderID = shaderId;

    /////////
    Light light = {
        glm::vec3(0.2f, 0.2f, 0.2f), // light position
        glm::vec3(0.7f, 0.7f, 0.7f), // ambient white
        glm::vec3(0.2f, 0.2f, 0.2f), // diffuse white
        glm::vec3(1.0f, 0.0f, 1.0f), // specular
    };
    shader->setVec3("u_light.ambient", light.ambient);
    shader->setVec3("u_light.diffuse", light.diffuse);
    shader->setVec3("u_light.specular", light.specular);
    shader->setVec3("u_light.position", light.position);
    /////////

    /////////
    shader->setVec3("u_viewPos", activeCamera->Position);
    /////////

    /////////
    glm::mat4 modelM4 = obj.getModelMatrix();
    glm::vec3 position = obj.getPosition(); // glm::vec3(0.0f, 0.0f, 0.0f);
    modelM4 = glm::translate(modelM4, position); // move top-left
    shader->setMat4("u_Model", modelM4);
    shader->setVec3("u_material.ambient", obj.material.ambient);
    shader->setVec3("u_material.diffuse", obj.material.diffuse);
    shader->setVec3("u_material.specular", obj.material.specular);
    shader->setFloat("u_material.shininess", obj.material.shininess);
    /////////

    model->draw();
}