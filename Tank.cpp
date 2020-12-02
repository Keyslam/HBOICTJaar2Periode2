#include "Tank.hpp"

#include <iostream>

Vector2<float> Tank::targetBlue = Vector2<float>(1200, 600);
Vector2<float> Tank::targetRed = Vector2<float>(80, 80);

Tank::Tank() { }

Tank::Tank(Vector2<float> position, bool allignment, int health) :
	position(position.x, position.y),
	health(health),
	force(0, 0),
	active(true),
	allignment(allignment)
{

}
