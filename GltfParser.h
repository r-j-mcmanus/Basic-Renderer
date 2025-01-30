#pragma once

#include "nlohmann/json.hpp"
#include "DrawableBuffer.h"
#include <vector>
#include <memory>

using json = nlohmann::json;


class GltfParser {
public:
    std::shared_ptr<DrawableBuffer> parse(const std::string& filename);
private:
    struct VertexAttribData {
        int componentsPerVertex;
        int bufferIndex;
        int byteOffset;
        int dataType;  // the datatype index used in gltf is the same as that in glfw
        int byteStride;
        bool normalized;
        int count;
    };

    struct IndexData {
        const unsigned int VBO;
        const int count;
        const int dataType; // the datatype index used in gltf is the same as that in glfw
        int byteOffset;
    };

private:
    const std::vector<unsigned int> parseBuffers(json& data) const;

    const void setAttribute(int attribArray, int accessorIndex, json data, const std::vector<unsigned int> VBOs) const;
    DrawableBuffer::BufferData setIndices(int indicesAccessorIndex, json data, const unsigned int VAO, const std::vector<unsigned int> VBOs) const;

    const VertexAttribData getVertexAttribData(int accessorIndex, json data) const;
};