#pragma once

#include "Component.h"
#include "light.h"

class LightComponent {
public:
	LightComponent(const Light& light) : light(light) {}
private:
	Light light;
};