#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class DrawableBuffer {

public:
    struct BufferData {
        const unsigned int VBO;
        const int count;
        const int dataType; // the datatype index used in gltf is the same as that in glfw
        int byteOffset;
    };

public:
    const void draw() const {
        for(auto& buffer : buffers)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.VBO);

            glDrawElements(
                GL_TRIANGLES,
                buffer.count,
                buffer.dataType,
                reinterpret_cast<void*>(buffer.byteOffset)
            );
        }
    }

public:
    void addBuffer(BufferData bufferData) {
        buffers.push_back(bufferData);
    }

private:
    std::vector<BufferData> buffers;
};