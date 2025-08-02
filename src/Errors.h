#pragma once

#include <GL/glew.h>
#include <AL/al.h>

#include <iostream>


// ==================================================================
// OpenGL
// ==================================================================

// macros for error logging openGL
// #x returns the litteral string eg #h = "foo"
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/*
Clears the GL error buffer
*/
void GLClearError();

/*
CleLogs any errors in the GL error buffer
*/
bool GLLogCall(const char* function, const char* file, int line);


// ==================================================================
// OpenAL
// ==================================================================
#define ASSERT(x) if (!(x)) __debugbreak();
#define ALCall(x) ALClearError();\
    x;\
    ASSERT(ALLogCall(#x, __FILE__, __LINE__))


void ALClearError();
bool ALLogCall(const char* function, const char* file, int line);