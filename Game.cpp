#include "Game.hpp"

// #include <glad/glad.h>
#include <SDL.h>

#include <iostream>

#include "Instrumentor.hpp"

#include "Vector2.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
#include <limits>

Game::Game()
{
	renderer = new Renderer();
	
	tshBlue = new TankSpatialHash(VIRTUAL_GAME_WIDTH, VIRTUAL_GAME_HEIGHT);
	tshRed = new TankSpatialHash(VIRTUAL_GAME_WIDTH, VIRTUAL_GAME_HEIGHT);
}

Game::~Game()
{
	delete renderer;

	delete tshBlue;
	delete tshRed;
}

bool Game::Init()
{
	PROFILE_FUNCTION();

	if (!renderer->Init())
	{
		return false;
	}

	const Vector2<float> tankSize(14, 18);

	const unsigned int numTanksBlue = 1279;
	const unsigned int numTanksRed = 1279;

	const unsigned int rows = (unsigned int)sqrt(numTanksBlue + numTanksRed);
	const unsigned int maxRows = 12;

	const Vector2<float> startBlue = tankSize + Vector2<float>(10.0f, 80.0f);
	const Vector2<float> startRed = Vector2<float>(980.0f, 100.0f);

	const float spacing = 15.0f;

	for (int i = 0; i < 1279; i++)
	{
		{
			const float x = startBlue.x + ((i % maxRows) * spacing);
			const float y = startBlue.y + ((i / maxRows) * spacing);

			tanksBlue[i].position.Set(x, y);
			tanksBlue[i].allignment = true;
			tanksBlue[i].health = 10;

			tshBlue->AddTank(tanksBlue[i]);
		}

		{
			const float x = startRed.x + ((i % maxRows) * spacing);
			const float y = startRed.y + ((i / maxRows) * spacing);

			tanksRed[i].position.Set(x, y);
			tanksRed[i].allignment = false;
			tanksRed[i].health = 10;

			tshRed->AddTank(tanksRed[i]);
		}
	}

	return true;
}

void Game::Tick()
{
	// PROFILE_FUNCTION();

	TickBlueTanks(0, 1279);
	TickRedTanks(0, 1279);
	TickShooting();
	TickMoveRockets(0, rocketBlueCount);


	// Remove dead rockets
	for (int i = rocketBlueCount - 1; i >= 0; i--) {
		Rocket rocket = rocketsBlue[i];

		if (!rocket.alive) {
			if (i != rocketBlueCount - 1)
				rocketsBlue[i] = rocketsBlue[rocketBlueCount - 1];
				
			rocketBlueCount--;
		}
	}

	// Remove dead rockets
	for (int i = rocketRedCount - 1; i >= 0; i--) {
		Rocket rocket = rocketsRed[i];

		if (!rocket.alive) {
			if (i != rocketRedCount - 1)
				rocketsRed[i] = rocketsRed[rocketRedCount - 1];

			rocketRedCount--;
		}
	}

	static std::chrono::steady_clock::time_point lastTimeSinceRender = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::nanoseconds duration = (now - lastTimeSinceRender);
	std::chrono::milliseconds targetDuration = std::chrono::milliseconds(1000 / 60);

	if (duration >= targetDuration) {
		renderer->Clear();

		RenderBlueTanks(0, 1279);
		RenderBlueRockets(0, rocketBlueCount);

		RenderRedTanks(0, 1279);
		RenderRedRockets(0, rocketRedCount);

		renderer->Present();
		lastTimeSinceRender = now;
	}
}

void Game::TickBlueTanks(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Tank& tank = tanksBlue[i];

		Vector2<float> normalizedDirection = (Tank::targetBlue - tank.position);
		normalizedDirection.Normalize();

		Vector2<float> velocity = normalizedDirection + tank.force;

		tank.position += velocity * Tank::maxSpeed;

		tshBlue->UpdateTank(tank);
	}
}

void Game::TickRedTanks(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Tank& tank = tanksRed[i];

		Vector2<float> normalizedDirection = (Tank::targetRed - tank.position);
		normalizedDirection.Normalize();

		Vector2<float> velocity = normalizedDirection + tank.force;
		velocity *= Tank::maxSpeed;

		tank.position += velocity;

		tshRed->UpdateTank(tank);
	}
}

void Game::TickShooting() {
	framesUntilShoot--;

	if (framesUntilShoot <= 0) {
		for (int i = 0; i < 1279; i++) {
			Tank tank = tanksBlue[i];

			Rocket& rocket = rocketsBlue[rocketBlueCount + i];
			rocket.position.Set(tank.position.x, tank.position.y);

			Tank& target = FindClosestRedTank(tank.position);

			Vector2<float> targetPosition = (target.position - tank.position).Normalized();
			targetPosition *= 3.0f;

			rocket.velocity.Set(targetPosition.x, targetPosition.y);
		}
		rocketBlueCount += 1279;

		framesUntilShoot = TANK_SHOOT_COOLDOWN;
	}

	if (framesUntilShoot <= 0) {
		for (int i = 0; i < 1279; i++) {
			Tank tank = tanksRed[i];

			Rocket& rocket = rocketsRed[rocketRedCount + i];
			rocket.position.Set(tank.position.x, tank.position.y);
			
			Tank& target = FindClosestBlueTank(tank.position);

			Vector2<float> targetPosition = (target.position - tank.position).Normalized();
			targetPosition *= 3.0f;

			rocket.velocity.Set(targetPosition.x, targetPosition.y);
		}
		rocketRedCount += 1279;

		framesUntilShoot = TANK_SHOOT_COOLDOWN;
	}
}

void Game::TickMoveRockets(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Rocket& rocket = rocketsBlue[i];
		rocket.position += rocket.velocity;

		tshRed->EachOverlappingCell(rocket.position.x - Rocket::collisionRadius, rocket.position.y - Rocket::collisionRadius, Rocket::collisionRadius * 2, Rocket::collisionRadius * 2, [&](TankSpatialCell* cell) {
			for (int i = 0; i < cell->size(); i++) {
				Tank* tank = (*cell)[i];


			}
		});

		// Check > width first, because that is more likely
		rocket.alive = !((rocket.position.x > VIRTUAL_GAME_WIDTH) || (rocket.position.x < 0) || (rocket.position.y < 0) || (rocket.position.y > VIRTUAL_GAME_HEIGHT));
	}

	for (int i = start; i < start + count; i++) {
		Rocket& rocket = rocketsRed[i];
		rocket.position += rocket.velocity;

		// Check < 0 first, because that is more likely
		rocket.alive = !((rocket.position.x < 0) || (rocket.position.x > VIRTUAL_GAME_WIDTH) || (rocket.position.y < 0) || (rocket.position.y > VIRTUAL_GAME_HEIGHT));
	}
}

void Game::RenderBlueTanks(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Tank& tank = tanksBlue[i];
		renderer->DrawBlueTank(tank.position.x, tank.position.y);
	}
}

void Game::RenderBlueRockets(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Rocket& rocket = rocketsBlue[i];
		renderer->DrawBlueRocket(rocket.position.x, rocket.position.y);
	}
}

void Game::RenderRedTanks(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Tank& tank = tanksRed[i];
		renderer->DrawRedTank(tank.position.x, tank.position.y);
	}
}

void Game::RenderRedRockets(int start, int count) {
	PROFILE_FUNCTION();

	for (int i = start; i < start + count; i++) {
		Rocket& rocket = rocketsRed[i];
		renderer->DrawRedRocket(rocket.position.x, rocket.position.y);
	}
}

Tank& Game::FindClosestRedTank(Vector2<float> position) {
	Tank closest;
	float closestDistanceSquared = std::numeric_limits<float>::max();

	for (int i = 0; i < 1279; i++) {
		Tank tank = tanksRed[i];

		float distanceSquared = tank.position.Length2();

		if (distanceSquared < closestDistanceSquared) {
			closest = tank;
			closestDistanceSquared = distanceSquared;
		}
	}

	return closest;
}

Tank& Game::FindClosestBlueTank(Vector2<float> position) {
	Tank closest;
	float closestDistanceSquared = std::numeric_limits<float>::max();

	for (int i = 0; i < 1279; i++) {
		Tank tank = tanksBlue[i];

		float distanceSquared = tank.position.Length2();

		if (distanceSquared < closestDistanceSquared) {
			closest = tank;
			closestDistanceSquared = distanceSquared;
		}
	}

	return closest;
}