#pragma once

#include <vector>
#include <map>

#include "AABB.h"

class CollisionComponent;


class CollsionDetector {
// see https://gameprogrammingpatterns.com/spatial-partition.html

public:
	CollsionDetector(float cellWidth, float cellHeight, float cellDepth) : cellSize({ cellWidth, cellHeight, cellDepth }) {
	};

	std::vector<CollisionComponent*> getCollision(CollisionComponent* objects);

	std::vector<std::pair<CollisionComponent*, CollisionComponent*>> getCollisionPairs(std::vector<CollisionComponent*>& objects);

private:
	std::vector<std::pair<CollisionComponent*, CollisionComponent*>> getCollisionPairsFromHash();

	bool colides(const AABB a, const AABB b);

	void addObjectsToHash(std::vector<CollisionComponent*> objects);

	const size_t HashCell(const int x, const int y, const int z) const;

private:
	std::map<size_t, std::vector<CollisionComponent*>> hashMap;


	struct CellSize {
		float x;
		float y;
		float z;
	};

	CellSize cellSize = { 1.0f, 1.0f, 1.0f };
};