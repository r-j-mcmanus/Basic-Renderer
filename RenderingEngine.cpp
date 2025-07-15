
#include <memory>
#include <string>
#include <iostream>

#include "ShaderManager.h"
#include "ModelManager.h"

#include "Errors.h"
#include "RenderingEngine.h"
#include "Light.h"
#include "Shader.h"

#include "SceneNode.h"
#include "RenderableComponent.h"
#include "LightComponent.h"

#include "World.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void RenderingEngine::submitRenderRequests(SceneNode* node) {
    // Compute world transform
    //Transform worldTransform = parentTransform * node->localTransform;

    if (std::shared_ptr<RenderableComponent> renderable = node->getComponent<RenderableComponent>()) {
        const RenderData renderData = renderable->getDrawRequest();
        const glm::mat4 modelMatrix = node->GetGlobalMatrix();
        renderingQueue.push_back(RenderingQueueEntry{ renderData, modelMatrix });
    }
}


void RenderingEngine::getLights(SceneNode* node) {
    // Compute world transform
    //Transform worldTransform = parentTransform * node->localTransform;

    // If node has a RenderableComponent, submit it
    if (std::shared_ptr<LightComponent> renderable = node->getComponent<LightComponent>()) {
         // renderable->SubmitDrawRequest(this);
        int a = 1;
    }
}


// Main rendering function
void RenderingEngine::renderFrame(World& world)
{
    GLCall(glClearColor(clearColorR, clearColorG, clearColorB, clearColorA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    unsigned int currentShaderID = 0;

    SceneNode* sceneRoot = &(world.root);

    // todo using the camera we can cull objects
    sceneRoot->traverse([this](SceneNode* node) { submitRenderRequests(node); });
    sceneRoot->traverse([this](SceneNode* node) { getLights(node); });


    for (auto& it : renderingQueue) {
        const unsigned int modelId = it.renderData.modelID;
        const unsigned int shaderId = it.renderData.shaderID;

        std::shared_ptr<DrawableBuffer> model = modelManager->getBuffer(modelId);
        std::shared_ptr<Shader> shader = shaderManager->getShader(shaderId);

        if (!model || !shader) {
            std::cout << "Model or shader ID does not correspond to a reseource" << std::endl;
            continue;
        }

        shader->Bind();
        // currentShaderID = shaderId;

        /////////
        const glm::vec3 lightPosition = glm::vec3(0.2f, 0.2f, 0.2f); // light position
        Light light = {
            glm::vec4(0.7f, 0.7f, 0.7f, 0.0f), // ambient white
            glm::vec4(0.2f, 0.2f, 0.2f, 0.0f), // diffuse white
            glm::vec4(1.0f, 0.0f, 1.0f, 0.0f), // specular
        };
        shader->setVec3("u_light.ambient", light.ambient);
        shader->setVec3("u_light.diffuse", light.diffuse);
        shader->setVec3("u_light.specular", light.specular);
        shader->setVec3("u_light.position", lightPosition);
        /////////

        /////////
        shader->setVec3("u_viewPos", world.activeCameraNode->position);
        /////////

        /////////
        glm::mat4 modelM4 = it.modelMatrix;
        shader->setMat4("u_Model", modelM4);
        shader->setVec3("u_material.ambient", it.renderData.material.ambient);
        shader->setVec3("u_material.diffuse", it.renderData.material.diffuse);
        shader->setVec3("u_material.specular", it.renderData.material.specular);
        shader->setFloat("u_material.shininess", it.renderData.material.shininess);
        /////////

        model->draw();
    }

}
