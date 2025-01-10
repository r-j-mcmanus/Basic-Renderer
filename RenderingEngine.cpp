
#include <memory>
#include <string>

#include "World.h"
#include "Shader.h"
#include "WorldObject.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "ModelManager.h"

#include "RenderingEngine.h"

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

        if (shaderId != currentShaderID) {
            shader->Bind();
            currentShaderID = shaderId;
        }

        model->draw();
	}
}
