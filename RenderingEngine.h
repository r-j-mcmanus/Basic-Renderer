#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Material.h"
#include "Light.h"
#include "RenderableComponent.h"

class Camera;
class ShaderManager;
class ModelManager;
class UniformBufferManager;
class World;
struct GLFWwindow;
class SceneNode;

struct RenderingQueueEntry {
    RenderData renderData;
    glm::mat4 modelMatrix;
};

class RenderingEngine {
public:

    // Main rendering function
    void renderFrame(World& world);

    // basicly a callback foo used in the scene graph
    void addToRenderingQueue(RenderingQueueEntry entry) { renderingQueue.push_back(entry); };

    // Set the active camera
    void registerModelManager(const std::shared_ptr<ModelManager> modelManagerPtr) { modelManager = modelManagerPtr; };
    void registerShaderManager(const std::shared_ptr<ShaderManager> shaderManagerPtr) { shaderManager = shaderManagerPtr; };
    void registerUniformBufferManager(const std::shared_ptr<UniformBufferManager> uniformBufferManagerPtr) { uniformBufferManager = uniformBufferManagerPtr; };

private:
    void submitRenderRequests(SceneNode* node);
    void getLights(SceneNode* node);

private:
    std::shared_ptr<ShaderManager> shaderManager = nullptr;
    std::shared_ptr<ModelManager> modelManager = nullptr;
    std::shared_ptr<UniformBufferManager> uniformBufferManager = nullptr;

    const float clearColorR = 0.1f;
    const float clearColorG = 0.1f;
    const float clearColorB = 0.1f;
    const float clearColorA = 1.0f;

    std::vector<RenderingQueueEntry> renderingQueue;
    std::vector<Light> activeLights;
};
