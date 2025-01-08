#include "RenderingEngine.h"
#include "World.h"
#include "Shader.h"
#include "WorldObject.h"
#include <memory>
#include <string>

// Initialize the rendering engine
bool RenderingEngine::initialize(const std::shared_ptr<GLFWwindow> windowContext) {
    return true;
}

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

// Cleanup resources
void RenderingEngine::cleanup() {

}

void RenderingEngine::setActiveCamera(std::shared_ptr<Camera> camera) {

}

void RenderingEngine::registerModelManager(const std::shared_ptr<ModelManager> modelManagerPtr) 
{
	modelManager = modelManagerPtr;
}

void RenderingEngine::registerShaderManager(const std::shared_ptr<ShaderManager> shaderManagerPtr) 
{
	shaderManager = shaderManagerPtr;
}