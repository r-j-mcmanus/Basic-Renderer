#pragma once

#include "RenderingController.h"
#include "Camera.h"

#include <glm/glm.hpp>



class Basic6RenderingController : public RenderingController {
public:
    Basic6RenderingController()
    {
    }

    void draw(WorldObject& obj, std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager, std::shared_ptr<Camera> activeCamera);
};