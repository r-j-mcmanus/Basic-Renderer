#pragma once

#include <memory>

#include "Material.h"
#include "Component.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "RenderingController.h"
#include "Component.h"



class RenderableComponent: public Component {
    void draw(std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<ModelManager> modelManager, std::shared_ptr<Camera> activeCamera);
    void setRendererController(RenderingController* controller);

private:
    RenderingController* controller;
    Material material;
};