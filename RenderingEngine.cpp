
#include <memory>
#include <string>

#include "World.h"
#include "Shader.h"
#include "WorldObject.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "ModelManager.h"

#include "Errors.h"
#include "RenderingEngine.h"
#include "Light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Main rendering function
void RenderingEngine::renderFrame(World& world) const
{
    GLCall(glClearColor(clearColorR, clearColorG, clearColorB, clearColorA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    unsigned int currentShaderID = 0;



	for (WorldObject& worldObject : world.worldObjects)
	{
        const unsigned int modelId = worldObject.renderingComponent.modelID;
        const unsigned int shaderId = worldObject.renderingComponent.shaderID;

        std::shared_ptr<DrawableBuffer> model = modelManager->getBuffer(modelId);
        std::shared_ptr<Shader> shader = shaderManager->getShader(shaderId);

        if (!model || !shader) {
            std::cout << "Model or shader ID does not correspond to a reseource" << std::endl;
            continue;
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
        glm::mat4 modelM4 = worldObject.getModelMatrix();
        glm::vec3 position = worldObject.getPosition(); // glm::vec3(0.0f, 0.0f, 0.0f);
        modelM4 = glm::translate(modelM4, position); // move top-left
        shader->setMat4("u_Model", modelM4);
        shader->setVec3("u_material.ambient", worldObject.material.ambient);
        shader->setVec3("u_material.diffuse", worldObject.material.diffuse);
        shader->setVec3("u_material.specular", worldObject.material.specular);
        shader->setFloat("u_material.shininess", worldObject.material.shininess);
        /////////
         
        model->draw();
	}
}
