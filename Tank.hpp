#pragma once

#include "Vector2.hpp"
#include "TankSpatialHashCache.hpp"

class Tank
{
public:
	const static int collisionRadius = 9;
	const static int maxSpeed = 1;

	static Vector2<float> targetBlue;
	static Vector2<float> targetRed;

	Tank();
	Tank(Vector2<float> position, bool allignment, int health);

	Vector2<float> position;

	int health;

	Vector2<float> force;

	TankSpatialHashCache spatialHashCache;

	bool active;

	bool allignment;
private:
};