#pragma once

#include "WorldObject.h"

class WorldObject;

class MovementController {
public:
    virtual ~MovementController() = default;
    virtual void update(WorldObject& obj, float deltaTime) = 0;
};