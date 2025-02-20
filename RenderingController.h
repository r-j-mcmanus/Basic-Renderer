#pragma once

#include "memory"

#include "WorldObject.h"

#include "ShaderManager.h"
#include "ModelManager.h"
#include "Camera.h"


class WorldObject;

class RenderingController {
public:
    virtual ~RenderingController() = default;
    virtual void draw(WorldObject& obj, std::shared_ptr <ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager, std::shared_ptr<Camera> activeCamera) = 0;
};