#pragma once
#include "Vector2.hpp"

class Rocket
{
public:
	const static int collisionRadius = 10;

	Rocket();
	Rocket(Vector2<float>& position, Vector2<float>& velocity);

	Vector2<float> position;
	Vector2<float> velocity;
	bool alive = true;
};

