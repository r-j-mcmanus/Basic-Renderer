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

    bool draw(const unsigned int id) {
        auto modelIt = DrawableBufferlMap.find(id);
        if (modelIt != DrawableBufferlMap.end())
        {
            modelIt->second->draw();
            return true;
        }
        return false;
    };

private:
    // to do use model id as the thing we call things and assign model id to a path so that if we access a model id that doesnt exist we load it.
    // will need a hardcoded list of objects to ids
    // probably make a program that makes that list by scanning a dir?
    unsigned int nextRenderID = 0;
    std::unordered_map<unsigned int, std::shared_ptr<DrawableBuffer>> DrawableBufferlMap;
    std::unordered_map<std::string, unsigned int> modelPathMap;
};
