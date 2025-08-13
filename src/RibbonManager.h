#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "Errors.h"
#include "RibbonBufferData.h"

// todo move from multiple vbos to a single vbo where we parse sections out and render in a single draw call
class RibbonManager {
public:
	// makes maxRibbons number of VBOs of size maxRibbonQuads
	RibbonManager() {
		for (unsigned int i = 0; i < MAX_RIBBONS; i++) {
			RibbonBufferData ribbonBufferData = bufferData[i];
			GLCall(glGenVertexArrays(1, &ribbonBufferData.vao));
			GLCall(glGenBuffers(1, &ribbonBufferData.vbo));
			GLCall(glBindVertexArray(ribbonBufferData.vao));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, ribbonBufferData.vbo));
			GLCall(glBufferData(GL_ARRAY_BUFFER, MAX_RIBBON_QUADS * 4 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW));
			GLCall(glEnableVertexAttribArray(0));
			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
			bufferData[i] = ribbonBufferData;
		}
	};

	~RibbonManager() {
		for (RibbonBufferData ribbonRenderData : bufferData) {
			GLCall(glDeleteBuffers(1, &ribbonRenderData.vbo));
			GLCall(glDeleteVertexArrays(1, &ribbonRenderData.vao));
		}
	};

	RibbonBufferData* getRibbonBuffer() {
		for (unsigned int idx = 0; idx < MAX_RIBBONS; idx++) {
			if (!bufferData[idx].inUse) {
				bufferData[idx].inUse = true;
				return &bufferData[idx];
			}
		}
		return nullptr;
	}

	void draw(RibbonBufferData* ribbonRenderData, const std::vector<glm::vec3>& vertices) {
		if (ribbonRenderData) {
			GLCall(glBindVertexArray(ribbonRenderData->vao));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, ribbonRenderData->vbo));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data()));
			GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size()));
		}
	}

	GLuint shaderRibbonId = 0;	

private:
	unsigned int count = 0;
	RibbonBufferData bufferData[MAX_RIBBONS];
};