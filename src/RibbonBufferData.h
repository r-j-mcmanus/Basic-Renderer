#pragma once

#include <GL/glew.h>

static const unsigned int MAX_RIBBONS = 3;
static const unsigned int MAX_RIBBON_QUADS= 50;


struct RibbonBufferData {
	GLuint vbo = 0;
	GLuint vao = 0;
	bool inUse= false;
};