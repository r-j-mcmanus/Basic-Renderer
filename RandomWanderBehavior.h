#pragma once

#include <random>
#include <cmath>

#include <glm/vec3.hpp>

#include "AIBehavior.h"


class RandomWanderBehavior : public AIBehavior {
public:
    void onEnter(SceneNode* node) override {
        if (node) {
            originalPosition = node->getGlobalPosition();
            generateNewTarget();
        }
    }

    void update(SceneNode* node, float dt) override {
        if (!node) return;

        glm::vec3 pos = node->getGlobalPosition();

        const float speed = 0.5f;
        glm::vec3 dir = {
            targetPosition.x - pos.x,
            targetPosition.y - pos.y,
            targetPosition.z - pos.z
        };
        float distSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;

        if (distSq < 0.01f) {
            generateNewTarget();
        }

        float len = std::sqrt(distSq);
        dir.x /= len;
        dir.y /= len;
        dir.z /= len;

        node->translate({
            dir.x * speed * dt,
            dir.y * speed * dt,
            dir.z * speed * dt
        });
    }

private:
    glm::vec3 originalPosition;
    glm::vec3 targetPosition;

    void generateNewTarget() {
        static std::default_random_engine eng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        float dx = dist(eng), dy = dist(eng), dz = dist(eng);
        float lenSq = dx * dx + dy * dy + dz * dz;
        if (lenSq > 1.0f) {
            float len = std::sqrt(lenSq);
            dx /= len;
            dy /= len;
            dz /= len;
        }

        targetPosition = originalPosition + glm::vec3(dx, dy, dz);
    }
};