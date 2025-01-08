#pragma once

#include <string>
#include <vector>
#include <memory>

#include "World.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"


// RenderingEngine class declaration
class RenderingEngine {
public:
    // Constructor and Destructor
    // RenderingEngine();
    // ~RenderingEngine();

    // Initialize the rendering engine
    bool initialize(const std::shared_ptr<GLFWwindow> windowContext);

    // Main rendering function
    void renderFrame(World& world) const;

    // Cleanup resources
    void cleanup();

    // Set the active camera
    void setActiveCamera(std::shared_ptr<Camera> camera);

    void registerModelManager(const std::shared_ptr<ModelManager> modelManagerRef);
    void registerShaderManager(const std::shared_ptr<ShaderManager> shaderManagerRef);

private:
    void* windowContext;
    std::shared_ptr<ShaderManager> shaderManager = nullptr;
    std::shared_ptr<ModelManager> modelManager = nullptr;

    // Active camera
    std::shared_ptr<Camera> activeCamera;

    const float clearColorR = 0.1f;
    const float clearColorG = 0.1f;
    const float clearColorB = 0.1f;
    const float clearColorA = 1.0f;
};
