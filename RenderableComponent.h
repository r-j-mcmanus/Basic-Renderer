#pragma once

#include <memory>

#include "Material.h"
#include "Component.h"


struct RenderData {
    Material material;
    unsigned int modelID;
    unsigned int shaderID;
};


class RenderableComponent: public Component {

public:
    RenderableComponent(unsigned int modelID, unsigned int shaderID) : modelID(modelID), shaderID(shaderID) {};

    const RenderData getDrawRequest()
    {
        return RenderData { material , modelID, shaderID };
    }

private:
    Material material;
    unsigned int modelID;
    unsigned int shaderID;
};