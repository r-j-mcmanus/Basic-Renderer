#pragma once

#include "memory"

#include "ShaderManager.h"
#include "ModelManager.h"


class WorldObject;

class RenderingController {
public:
    virtual ~RenderingController() = default;
    virtual void draw(WorldObject& obj, std::shared_ptr <ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager) = 0;
};