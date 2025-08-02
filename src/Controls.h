#pragma once

#include <memory>

#include "KeyTracker.h"
#include "MouseTracker.h"
#include "SceneNode.h"

class Controls {
public:
    Controls(
        std::shared_ptr<KeyTracker> keyTracker, 
        std::shared_ptr<MouseTracker> mouseTracker
    ): keyTracker(keyTracker), mouseTracker(mouseTracker)
    {}

    void fixedUpdateKey(float dt) {};

protected:
    std::shared_ptr<KeyTracker> keyTracker;
    std::shared_ptr<MouseTracker> mouseTracker;
    SceneNode* parent = nullptr;
};