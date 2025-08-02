#pragma once

#include "Component.h"
#include "light.h"
#include "SceneNode.h"

class LightComponent: public Component {
public:
	LightComponent(const Light& light) : light(light) {}

	Light getLight() {
		light.position = glm::vec4(parent->getGlobalPosition(), 0);
		return light;
	}
private:
	Light light;
};