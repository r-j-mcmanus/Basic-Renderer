#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Errors.h"

class DrawableBuffer {

public:
    struct BufferData {
        const unsigned int VBO;
        const unsigned int VAO;
        const int count;
        const int dataType; // the datatype index used in gltf is the same as that in glfw
        int byteOffset;
    };

public:
    void draw() const {
        for(auto& buffer : buffers)
        {
            GLCall(glBindVertexArray(buffer.VAO));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO));
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.VBO));

            // Combined buffer: VBO stores both vertex and index data
            // this is as it is laied out as such in a gltf file
            GLCall(glDrawElements(
                GL_TRIANGLES,
                buffer.count,
                buffer.dataType,
                reinterpret_cast<void*>(static_cast<uintptr_t>(buffer.byteOffset))
                //reinterpret_cast<void*>(buffer.byteOffset)
            ));

            GLCall(glBindVertexArray(0)); // Unbind the VAO
        }
    }

public:
    void addBuffer(BufferData bufferData) {
        buffers.push_back(bufferData);
    }

private:
    std::vector<BufferData> buffers;
};