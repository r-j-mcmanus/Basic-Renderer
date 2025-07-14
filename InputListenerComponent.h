#pragma once

#include <memory>
#include <iostream>

#include "Component.h"
#include "EventListenerInterface.h"
#include "EventHandler.h"


class InputListenerComponent :
    public Component,
    public EventListenerInterface,
    public std::enable_shared_from_this<InputListenerComponent>
{
public:
    InputListenerComponent(std::shared_ptr<EventHandler> handler)
        : eventHandler(handler) {
    }

    void onAttach(SceneNode& node) override {
        this->owner = &node;
        if (auto handler = eventHandler.lock()) {
            handler->registerObserver(shared_from_this());
        }
    }

    void onDetach(SceneNode& node) override {
        if (auto handler = eventHandler.lock()) {
            handler->unregisterObserver(shared_from_this());
        }
    }

    void onKeyEvent(int key, int scancode, int action, int mods) override {
        // Example logic
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            std::cout << "Spacebar pressed on node!" << std::endl;
        }
    }

    // Implement other callbacks if needed...

private:
    std::weak_ptr<EventHandler> eventHandler;
    SceneNode* owner = nullptr; // Store the owning node for context
};
