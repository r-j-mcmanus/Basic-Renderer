#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Material.h"
#include "Light.h"
#include "RenderableComponent.h"

#include "RibbonManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "UniformBufferManager.h"

#include "ShaderData.h"

class Camera;
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

private:
    void submitRenderRequests(SceneNode* node);
    void getLights(SceneNode* node);

    void renderRibbon(SceneNode* node, glm::vec3& cameraPosition, glm::vec3& cameraDirection);
    void renderStaticMesh(glm::vec3& viewPosition);

public:
    RibbonManager ribbonManager;
    ShaderManager shaderManager;
    ModelManager modelManager;
    UniformBufferManager uniformBufferManager;

private:
    const float clearColorR = 0.1f;
    const float clearColorG = 0.1f;
    const float clearColorB = 0.1f;
    const float clearColorA = 1.0f;

    std::vector<RenderingQueueEntry> renderingQueue;
    std::vector<Light> activeLights;
};
