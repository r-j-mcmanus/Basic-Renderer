
#include <memory>
#include <string>
#include <iostream>

#include "ShaderManager.h"
#include "ModelManager.h"
#include "UniformBufferManager.h"

#include "Errors.h"
#include "RenderingEngine.h"
#include "Light.h"
#include "Shader.h"

#include "SceneNode.h"
#include "RenderableComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"

#include "World.h"

#include "helper.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void RenderingEngine::submitRenderRequests(SceneNode* node) {
    // Compute world transform
    //Transform worldTransform = parentTransform * node->localTransform;

    if (std::shared_ptr<RenderableComponent> renderable = node->getComponent<RenderableComponent>()) {
        const RenderData renderData = renderable->getDrawRequest();
        const glm::mat4 modelMatrix = node->getGlobalMatrix();
        renderingQueue.push_back(RenderingQueueEntry{ renderData, modelMatrix });
    }
}


void RenderingEngine::getLights(SceneNode* node) {
    if (std::shared_ptr<LightComponent> lightComponent = node->getComponent<LightComponent>()) {
         // renderable->SubmitDrawRequest(this);
        activeLights.push_back(lightComponent->getLight());
    }
}


// Main rendering function
void RenderingEngine::renderFrame(World& world)
{
    GLCall(glClearColor(clearColorR, clearColorG, clearColorB, clearColorA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // todo using the camera we can cull objects
    renderingQueue.clear();
    activeLights.clear();

    world.root.traverse([this](SceneNode* node) { submitRenderRequests(node); });
    world.root.traverse([this](SceneNode* node) { getLights(node); });

    // the buffers are shared between shaders and are linked elsewhere
    glm::mat4 view = world.activeCameraNode->getComponent<CameraComponent>()->getViewMatrix();

    uniformBufferManager->updateBuffer("ProjectionView", view, sizeof(glm::mat4));
    uniformBufferManager->updateBuffer("Lights", activeLights, 0);

    for (auto& it : renderingQueue) {
        const unsigned int modelId = it.renderData.modelID;
        const unsigned int shaderMapId = it.renderData.shaderMapID;

        std::shared_ptr<DrawableBuffer> model = modelManager->getBuffer(modelId);
        std::shared_ptr<Shader> shader = shaderManager->getShader(shaderMapId);

        if (!model || !shader) {
            std::cout << "Model or shader ID does not correspond to a reseource" << std::endl;
            continue;
        }

        // bind the shader we are to use for the model
        shader->Bind();

        // set uniforms for the shader

        /////////
        // these are shader dependent, so we should come up with a better palce for them to live
        /////////
        shader->setVec4("u_Color", glm::vec4(0.5));
        /////////
        glm::mat4 modelM4 = it.modelMatrix;
        shader->setMat4("u_Model", modelM4);
        /*shader->setVec3("u_material.ambient", it.renderData.material.ambient);
        shader->setVec3("u_material.diffuse", it.renderData.material.diffuse);
        shader->setVec3("u_material.specular", it.renderData.material.specular);
        shader->setFloat("u_material.shininess", it.renderData.material.shininess);*/
        /////////

        shader->validate();
        model->draw();
    }
}
