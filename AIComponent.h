#pragma once

#include "Component.h"
#include "AIBehavior.h"

class AIComponent : public Component {
public:
    explicit AIComponent(std::unique_ptr<AIBehavior> behavior)
        : behavior(std::move(behavior)) {
    }

    void onBuild(SceneNode& node) override {
        parent = &node;
        if (behavior) behavior->onEnter(parent);
    }

    void fixedUpdate(float dt) override {
        if (behavior && parent) {
            behavior->update(parent, dt);
        }
    }

    void setBehavior(std::unique_ptr<AIBehavior> newBehavior) {
        behavior = std::move(newBehavior);
    }

private:
    std::unique_ptr<AIBehavior> behavior;
};