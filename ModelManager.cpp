#include "Errors.h"

#include "ModelManager.h"
#include <string>
#include <glm/glm.hpp>

#include "GltfParser.h"

// Cleanup resources
void ModelManager::cleanup() {
	DrawableBufferMap.clear();
	modelPathMap.clear();
	nextRenderID = 0;
}

unsigned int ModelManager::loadModel(const std::string& modelPath)
{
	auto modelIt = modelPathMap.find(modelPath);
	if (modelIt != modelPathMap.end())
	{
		return modelIt->second;
	}

	 GltfParser gltfParser;

	unsigned int renderID = nextRenderID++;
	DrawableBufferMap[renderID] = gltfParser.parse(modelPath);
    modelPathMap[modelPath] = renderID;

	return renderID;
}

std::shared_ptr<DrawableBuffer> ModelManager::getBuffer(const unsigned int modelId) const {
	return DrawableBufferMap.at(modelId);
}
