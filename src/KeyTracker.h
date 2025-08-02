#pragma once

#include <GLFW/glfw3.h>

#include "EventListenerInterface.h"


class KeyTracker : public EventListenerInterface {
public:
	bool isKeyDown(int key) const { return keys[key]; }
	bool isKeyUp(int key) const { return !keys[key]; }

private:
	bool keys[1024] = { false }; // Track keys (true = pressed)

protected:
	void onKeyEvent(int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) keys[key] = true;
		if (action == GLFW_RELEASE) keys[key] = false;
	}
};