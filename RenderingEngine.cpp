
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Main rendering function
void RenderingEngine::renderFrame(World& world) const
{
    GLCall(glClearColor(clearColorR, clearColorG, clearColorB, clearColorA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    unsigned int currentShaderID = 0;

	for (const WorldObject& worldObject : world.worldObjects)
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
        glm::mat4 modelM4 = worldObject.getModelMatrix();
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        modelM4 = glm::translate(modelM4, position); // move top-left
        shader->setMat4("u_Model", modelM4);
        shader->setVec4("u_Color", float(255) / 255, float(127)/ 255, float(80)/ 255, 1);
        /////////

        model->draw();
	}
}
