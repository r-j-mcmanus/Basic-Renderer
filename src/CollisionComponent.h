#pragma once

#include "Component.h"
#include "AABB.h"

class CollisionComponent : public Component {
public:
	CollisionComponent() {};

	AABB getBoundingBox() { return aabb; };
private:
	AABB aabb;
};