#include "Rocket.hpp"

Rocket::Rocket() { }

Rocket::Rocket(Vector2<float>& position, Vector2<float>& velocity) :
	position(position.x, position.y),
	velocity(velocity.x, velocity.y)
{ }