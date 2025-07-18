#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "Errors.h"

class UniformBufferManager {
public:
    struct BufferInfo {
        GLuint bufferID;
        GLuint bindingPoint;
        size_t size;
    };

    // Create a new uniform buffer with a specific size and binding point
    void createBuffer(const std::string& bufferName, size_t size, GLuint bindingPoint) {
        if (buffers.find(bufferName) != buffers.end()) {
            std::cerr << "Buffer with name " << bufferName << " already exists.\n";
            return;
        }

        GLuint bufferID;
        GLCall(glGenBuffers(1, &bufferID));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, bufferID));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
        GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        buffers[bufferName] = { bufferID, bindingPoint, size };
    }

    // Bind a uniform block in a shader to a specific buffer
    void bindBlockToShader(GLuint shaderID, const std::string& blockName, const std::string& bufferName) {
        auto it = buffers.find(bufferName);
        if (it == buffers.end()) {
            std::cerr << "Buffer with name " << bufferName << " not found.\n";
            return;
        }

        GLuint blockIndex;
        GLCall(blockIndex = glGetUniformBlockIndex(shaderID, blockName.c_str()));
        if (blockIndex == GL_INVALID_INDEX) {
            std::cerr << "Uniform block " << blockName << " not found in shader.\n";
            return;
        }

        GLCall(glUniformBlockBinding(shaderID, blockIndex, it->second.bindingPoint));
    }

    // Update data in a uniform buffer
    template <typename T>
    void updateBuffer(const std::string& bufferName, const T& data, size_t offset) {
        auto it = buffers.find(bufferName);
        if (it == buffers.end()) {
            std::cerr << "Buffer with name " << bufferName << " not found.\n";
            return;
        }

        const BufferInfo& bufferInfo = it->second;
        if (offset + sizeof(T) > bufferInfo.size) {
            std::cerr << "Data size exceeds remaining buffer size for buffer " << bufferName << ".\n";
            return;
        }

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, bufferInfo.bufferID));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), glm::value_ptr(data)));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    // Update data in a uniform buffer with a vector
    template <typename T>
    void updateBuffer(const std::string& bufferName, const std::vector<T>& data, size_t offset) {
        auto it = buffers.find(bufferName);
        if (it == buffers.end()) {
            std::cerr << "Buffer with name " << bufferName << " not found.\n";
            return;
        }

        const BufferInfo& bufferInfo = it->second;
        const size_t dataSize = data.size() * sizeof(T);
        if (offset + dataSize > bufferInfo.size) {
            std::cerr << "Data size exceeds remaining buffer size for buffer " << bufferName << ".\n";
            return;
        }

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, bufferInfo.bufferID));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data.data()));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    ~UniformBufferManager() {
        for (const auto& pair : buffers) {
            GLCall(glDeleteBuffers(1, &pair.second.bufferID));
        }
    }

private:
    std::unordered_map<std::string, BufferInfo> buffers;
};

// Example Usage:
/*
struct ExampleData {
    float value1;
    int value2;
    float array[4];
};

UniformBufferManager ubm;
ubm.createBuffer("GlobalData", sizeof(ExampleData), 0);

GLuint shaderID = ...; // Obtain your shader ID here
ubm.bindBlockToShader(shaderID, "GlobalDataBlock", "GlobalData");

ExampleData data = {1.0f, 42, {0.0f, 1.0f, 2.0f, 3.0f}};
ubm.updateBuffer("GlobalData", data);
*/