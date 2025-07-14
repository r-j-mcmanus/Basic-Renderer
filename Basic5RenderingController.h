#pragma once

#include "RenderingController.h"
#include <glm/glm.hpp>



class Basic5RenderingController : public RenderingController {
public:
    Basic5RenderingController()
    {
    }

    void draw(WorldObject& obj, std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager);
};