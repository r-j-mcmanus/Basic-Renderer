#pragma once

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include "EventListenerInterface.h"

class EventHandler {
private:
    std::vector<std::weak_ptr<EventListenerInterface>> observers; // use weak_ptr to avoid dangling pointers

public:
    EventHandler(GLFWwindow* window);

    void registerObserver(std::shared_ptr<EventListenerInterface> observer);

    void unregisterObserver(std::shared_ptr<EventListenerInterface> observer);

    void notifyKeyEvent(int key, int scancode, int action, int mods);

    void notifyMouseEvent(int button, int action, int mods);

    void notifyMouseMovement(double xpos, double ypos);

    void notifyWindowResize(int width, int height);

    void cleanExpiredObservers();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyKeyEvent(key, scancode, action, mods);
    }

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyMouseEvent(button, action, mods);
    }

    static void mouseMovement(GLFWwindow* window, double xpos, double ypos) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyMouseMovement(xpos, ypos);
    }

    static void windowResizeCallback(GLFWwindow* window, int width, int height) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyWindowResize(width, height);
    }

private:
    bool hasFocus = true; // as a newly created window is given focus

    static void windowFocusCallback(GLFWwindow* window, int focused)
    {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->hasFocus = focused;
    }
};