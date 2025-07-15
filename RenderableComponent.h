#pragma once

#include <memory>

#include "Material.h"
#include "Component.h"
#include "RenderingEngine.h"


class RenderableComponent: public Component {

public:
    RenderableComponent(unsigned int modelID, unsigned int shaderID) : modelID(modelID), shaderID(shaderID) {};

    const void SubmitDrawRequest(RenderingEngine* renderingEngine)
    {
        renderingEngine->addToRenderingQueue({ material , modelID, shaderID });
    }

private:
    Material material;
    unsigned int modelID;
    unsigned int shaderID;
};