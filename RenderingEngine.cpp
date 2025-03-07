
#include <memory>
#include <string>

#include "World.h"
#include "WorldObject.h"
#include "ShaderManager.h"
#include "ModelManager.h"

#include "Errors.h"
#include "RenderingEngine.h"
#include "Light.h"

#include <glm/glm.hpp>

// Main rendering function
void RenderingEngine::renderFrame(World& world) const
{
    GLCall(glClearColor(clearColorR, clearColorG, clearColorB, clearColorA));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    unsigned int currentShaderID = 0;

	for (WorldObject& worldObject : world.worldObjects)
	{
        worldObject.draw(shaderManager, modelManager, activeCamera);
	}
}
