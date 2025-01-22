#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "Camera.h"
#include "ShaderManager.h"
#include "EventHandler.h"
#include "ModelManager.h"
#include "RenderingEngine.h"
#include "World.h"
#include "UniformBufferManager.h"

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

    /* can call glfwInit after creating valid context */
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}


std::shared_ptr<Camera> make_camera()
{
    // maybe read from init?
    glm::vec3 position = glm::vec3(3, 0, 3); // Camera pos in World Space
    glm::vec3 viewDirection = glm::vec3(0, 0, 0); // and looks at the origin
    glm::vec3 up = glm::vec3(0, 1, 0);  // Head is up (set to 0,-1,0 to look upside-down)
    const float fov_deg = 45.0f;
    const float aspectRatio = (float)640 / (float)480;
    const float nearPlane = 0.1f;
    const float farPlane = 100.0f;
    auto camera = std::make_shared<Camera>(position, viewDirection, up, fov_deg, aspectRatio, nearPlane, farPlane);
    camera->setTarget(glm::vec3(0, 0, 0));
    return camera;
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


    std::shared_ptr<Camera> camera = make_camera();
    std::shared_ptr<ShaderManager> shaderManager = std::make_shared<ShaderManager>();
    std::shared_ptr<UniformBufferManager> uniformBufferManager = std::make_shared<UniformBufferManager>();
    std::shared_ptr<ModelManager> modelManager = std::make_shared<ModelManager>();

    EventHandler eventHandler(window);
    eventHandler.registerObserver(camera);

    RenderingEngine renderingEngine;

    renderingEngine.registerModelManager(modelManager);
    renderingEngine.registerShaderManager(shaderManager);

    World world;
    world.buildWorld(1, modelManager, shaderManager, uniformBufferManager);

    double previousTime = glfwGetTime(); // Get the initial time
    GLCall(glEnable(GL_DEPTH_TEST));

    ///////
    /* Abstract this away */
    uniformBufferManager->createBuffer("ProjectionView", 2 * sizeof(glm::mat4), 0);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
    uniformBufferManager->updateBuffer("ProjectionView", projection, 0);

    unsigned int shaderID = shaderManager->getShader("basic")->GetId();
    uniformBufferManager->bindBlockToShader(shaderID, "Matrices", "ProjectionView");
    ///////

    /* Loop until the user closes the window */ 
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - previousTime;
        previousTime = currentTime;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ///////
        /* Abstract this away */
        glm::mat4 view = camera->getViewMatrix();
        uniformBufferManager->updateBuffer("ProjectionView", view, sizeof(view));
        ///////

        renderingEngine.renderFrame(world);

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }

    glfwTerminate();
    return 0;
}