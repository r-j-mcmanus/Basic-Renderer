#pragma once

#include <GL/glew.h>
#include <AL/al.h>

#include <iostream>


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

#define OpenAL_ErrorCheck(message)\
{\
    ALenum error = glGetError();\
    if (error != AL_NO_ERROR)\
    {\
        std::cerr << "OpenAL Error: " << error << std::endl;\
    }\
}

#define ALCall(x)\
x;\
OpenAL_ErrorCheck(x);