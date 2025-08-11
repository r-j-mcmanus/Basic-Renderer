#pragma once

#include <vector>
#include <map>

#include "AABB.h"
#include "CollsionDetector.h"
#include "CollisionComponent.h"


std::vector<std::pair<CollisionComponent*, CollisionComponent*>> CollsionDetector::getCollisionPairs(std::vector<CollisionComponent*> objects) {
	hashMap.clear();
	addObjectsToHash(objects);
	std::vector<std::pair<CollisionComponent*, CollisionComponent*>> collisionPairs = getCollisionPairsFromHash();
	return collisionPairs;
}


std::vector<CollisionComponent*> CollsionDetector::getCollision(CollisionComponent* obj) {
	std::vector<CollisionComponent*> collisions;
	
	AABB box = obj->getBoundingBox();
	int minX = floor(box.min.x / cellSize.x);
	int minY = floor(box.min.y / cellSize.y);
	int minZ = floor(box.min.z / cellSize.z);
	int maxX = floor(box.max.x / cellSize.x);
	int maxY = floor(box.max.y / cellSize.y);
	int maxZ = floor(box.max.z / cellSize.z);

	// add to all boxes that the AABB colides with
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			for (int z = minZ; z <= maxZ; z++) {
				size_t hash = HashCell(x, y, z);
				if (hashMap.contains(hash)) {
					auto& bucket = hashMap[hash];
					collisions.insert(std::end(collisions), std::begin(bucket), std::end(bucket));
				}
				hashMap[hash].push_back(obj);
			}
		}
	}
}

std::vector<std::pair<CollisionComponent*, CollisionComponent*>> CollsionDetector::getCollisionPairsFromHash() {
	std::vector<std::pair<CollisionComponent*, CollisionComponent*>> collisionPairs;

	//todo only check pairs once

	for (auto& [cellHash, objects] : hashMap) {
		size_t count = objects.size();

		for (size_t i = 0; i < count; i++) {
			for (size_t j = i + 1; j < count; j++) {
				if (colides(objects[i]->getBoundingBox(), objects[j]->getBoundingBox())) {
					collisionPairs.push_back({ objects[i] , objects[j] });
				}
			}
		}
	}

	// todo fine grained check

	return collisionPairs;
}

bool CollsionDetector::colides(AABB& a, AABB& b) {
	// No overlap if one is completely to the left/right
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	// No overlap if one is completely above/below
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	// No overlap if one is completely in front/behind
	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;

	return true; // They overlap on all three axes
}

void CollsionDetector::addObjectsToHash(std::vector<CollisionComponent*> objects) {

	for (auto obj : objects) {
		AABB box = obj->getBoundingBox();
		int minX = floor(box.min.x / cellSize.x);
		int minY = floor(box.min.y / cellSize.y);
		int minZ = floor(box.min.z / cellSize.z);
		int maxX = floor(box.max.x / cellSize.x);
		int maxY = floor(box.max.y / cellSize.y);
		int maxZ = floor(box.max.z / cellSize.z);

		// add to all boxes that the AABB colides with
		for (int x = minX; x <= maxX; x++) {
			for (int y = minY; y <= maxY; y++) {
				for (int z = minZ; z <= maxZ; z++) {
					size_t hash = HashCell(x, y, z);
					hashMap[hash].push_back(obj);
				}
			}
		}
	}
}

const size_t CollsionDetector::HashCell(int x, int y, int z) const {
	// Large primes reduce collisions in hash space
	return size_t((x * 73856093) ^ (y * 19349663) ^ (z * 83492791));
}
