#pragma once

#include <memory>

#include "Material.h"
#include "Component.h"


struct RenderData {
    Material material;
    unsigned int modelID;
    unsigned int shaderMapID;
};

class RenderableComponent: public Component {

public:
    RenderableComponent(unsigned int modelID, unsigned int shaderMapID) : modelID(modelID), shaderMapID(shaderMapID), material(defaultMaterial) {};

    const RenderData getDrawRequest()
    {
        return RenderData { material , modelID, shaderMapID };
    }

private:
    Material material;
    unsigned int modelID;
    unsigned int shaderMapID;
};