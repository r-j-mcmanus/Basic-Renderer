#pragma once

#include <vector>
#include <deque>

#include <glm/glm.hpp>

class Ribbon {
public:
	void update(glm::vec3 frontPos, float dt) {

		// only update the ribbon position if it has moved a sufficent amount
		if (trail.empty() || glm::distance(frontPos, trail.end()->pos) > minTrailDif) {
			trail.push_back({
				frontPos,
				0.f
				});
		}
		
		// todo store normalised age

		// age all trail points
		for (auto& p : trail) {
			p.timeAlive += dt;
		}

		// remove oldest
		while (!trail.empty() && trail.front().timeAlive > maxLifetime) {
			trail.pop_front();
		}
	}

	std::vector<glm::vec3> makeRibbonMesh(glm::vec3 cameraDirection) {
		// todo use smoother update step
		// given velocities, we can fit a curve and check for the gradient , and add itermediate steps if needed

		const int trailLen = trail.size();
		std::vector<glm::vec3> verts;
		for (int i = 0; i < trailLen - 1; i++) {
			const glm::vec3 segmentDirection = glm::normalize(trail[i+1].pos - trail[i].pos);
			// we take the right so that the quad faces the camera unless segmentDirection is parallels
			const glm::vec3 right = glm::normalize(glm::cross(cameraDirection, segmentDirection));
			const glm::vec3 offset = width * right;
			verts.push_back(trail[i].pos - offset);
			verts.push_back(trail[i].pos + offset);
		}
	}

private:

	struct TrailPoint {
		glm::vec3 pos;
		float timeAlive;
	};

	float minTrailDif;
	float maxLifetime;
	float width;
	std::deque<TrailPoint> trail;
};