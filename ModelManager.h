#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "DrawableBuffer.h"

class ModelManager {
public:
    void cleanup();

    unsigned int loadModel(const std::string& modelPath);

    std::shared_ptr<DrawableBuffer> getBuffer(const unsigned int modelId) const;


private:
    unsigned int nextRenderID = 0;
    std::unordered_map<unsigned int, std::shared_ptr<DrawableBuffer>> DrawableBufferMap;
    std::unordered_map<std::string, unsigned int> modelPathMap;
};
