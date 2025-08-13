#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include <iostream>
#include <memory>

#include "Shader.h" // shit place to put this, but is causing build errors somewhere if removed
#include "Audio.h"
#include "EventHandler.h"
#include "KeyTracker.h"
#include "MouseTracker.h"
#include "RenderingEngine.h"
#include "World.h"

#include "Errors.h"


static GLFWwindow* InitializeWindow(int width, int height, const char* title) 
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
    if (glfwRawMouseMotionSupported())
        GLCall(glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE));


    /* can call glfwInit after creating valid context */
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}


int main(void)
{
    GLFWwindow* window = InitializeWindow(640, 480, "Hello World");

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* message, const void* userParam) {
            std::cerr << "OpenGL Debug: " << message << "\n";
        }, nullptr);

    /////

    EventHandler eventHandler(window);
    std::shared_ptr<KeyTracker> keyTracker = std::make_shared<KeyTracker>();
    std::shared_ptr<MouseTracker> mouseTracker = std::make_shared<MouseTracker>();
    eventHandler.registerObserver(keyTracker);
    eventHandler.registerObserver(mouseTracker);

    RenderingEngine renderingEngine;

    World world;
    world.buildWorld(1, renderingEngine, keyTracker, mouseTracker);

    double previousTime = glfwGetTime(); // Get the initial time
    GLCall(glEnable(GL_DEPTH_TEST));

    const float dt = 1.0 / 120.0;
    double current_delta = 0;

    AudioManager audioManager;

    audioManager.createListener({ 0,0,0 }, { 0,0,0 });
    std::cout << "audio manager made" << std::endl;
    audioManager.loadWavFile("zap", "Resources/Sounds/320853__eloimarin__hello-world-chirp.wav");
    std::cout << "wav loaded" << std::endl;
    SourceInfo* source = audioManager.getFreeSource();
    std::cout << "got a free source" << std::endl;
    audioManager.bindBufferToMonoSource("zap", source);
    std::cout << "bound sound buffer to source" << std::endl;
    audioManager.playSource(source);
    std::cout << "finished playing sound" << std::endl;


    /* Loop until the user closes the window */ 
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        current_delta += currentTime - previousTime;
        previousTime = currentTime;
        while (current_delta > dt) {
            current_delta -= dt;
            world.fixedUpdate(dt);
            mouseTracker->resetDelta();
        }

        world.update();

        renderingEngine.renderFrame(world);

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }

    glfwTerminate();
    return 0;
}