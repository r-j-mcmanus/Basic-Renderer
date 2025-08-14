#pragma once

#include <memory>
#include <vector>
#include <deque>

#include <glm/glm.hpp>

#include "Component.h"
#include "SceneNode.h"
#include "RibbonBufferData.h"


class RibbonComponent : public Component {

public:
    RibbonComponent(RibbonBufferData* ribbonBufferdata = nullptr, float maxLifetime = 1.0f): maxLifetime(maxLifetime) {
		this->ribbonBufferData = ribbonBufferdata;
	};
    ~RibbonComponent() {
		ribbonBufferData->inUse = false;
	};

    void fixedUpdate(const float dt) {
		glm::vec3 frontPos = parent->getGlobalPosition();
		//printVec3(frontPos);
		// only update the ribbon position if it has moved a sufficent amount
		if (trail.empty() || glm::distance(frontPos, trail.back().pos) > minTrailDif) {
			trail.push_back({ frontPos, 0.f });
		}

		// todo store normalised age

		// age all trail points
		for (auto& p : trail) {
			p.timeAlive += dt;
		}

		// remove oldest
		while ((!trail.empty() && trail.front().timeAlive > maxLifetime) || trail.size() > MAX_RIBBON_QUADS) {
			trail.pop_front();
		}
	}

	std::vector<glm::vec3> makeRibbonMesh(glm::vec3 cameraDirection) {
		// todo use smoother update step
		// given velocities, we can fit a curve and check for the gradient , and add itermediate steps if needed

		//printVec3(cameraDirection);

		// todo we dont need the whole thing each step, we can store what was made and remove older edges
		std::vector<glm::vec3> verts;
		const int trailLen = trail.size();
		for (int i = 0; i < trailLen - 1; i++) {
			const glm::vec3 segmentDirection = glm::normalize(trail[i + 1].pos - trail[i].pos);
			// we take the right so that the quad faces the camera unless segmentDirection is parallels
			const glm::vec3 right = glm::normalize(glm::cross(cameraDirection, segmentDirection));
			const glm::vec3 offset = width * right * trail[i].timeAlive / maxLifetime;
			verts.push_back(trail[i].pos - offset);
			verts.push_back(trail[i].pos + offset);
		}
		return verts;
	};

public:
	RibbonBufferData* ribbonBufferData = nullptr;

private:
	struct TrailPoint {
		glm::vec3 pos;
		float timeAlive;
	};

	float minTrailDif = 0.01f;
	float maxLifetime = 1.0f;
	float width = 0.1f;
	std::deque<TrailPoint> trail;
};