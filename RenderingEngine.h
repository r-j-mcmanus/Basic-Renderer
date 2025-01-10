#pragma once

#include <string>
#include <vector>
#include <memory>

class Camera;
class ShaderManager;
class ModelManager;
class World;
class GLFWwindow;

class RenderingEngine {
public:

    // Main rendering function
    void renderFrame(World& world) const;

    // Set the active camera
    void setActiveCamera(std::shared_ptr<Camera> camera) { activeCamera = camera; };
    void registerModelManager(const std::shared_ptr<ModelManager> modelManagerPtr) { modelManager = modelManagerPtr; };
    void registerShaderManager(const std::shared_ptr<ShaderManager> shaderManagerPtr) { shaderManager = shaderManagerPtr; };

private:
    std::shared_ptr<ShaderManager> shaderManager = nullptr;
    std::shared_ptr<ModelManager> modelManager = nullptr;
    std::shared_ptr<Camera> activeCamera = nullptr;

    const float clearColorR = 0.1f;
    const float clearColorG = 0.1f;
    const float clearColorB = 0.1f;
    const float clearColorA = 1.0f;
};
