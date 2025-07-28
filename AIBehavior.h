#pragma once

#include "SceneNode.h"

// Define a base class for all AI behaviors
class AIBehavior {
public:
    virtual ~AIBehavior() = default;
    virtual void onEnter(SceneNode* node) {}
    virtual void update(SceneNode* node, float dt) = 0;
};