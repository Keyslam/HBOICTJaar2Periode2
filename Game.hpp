#pragma once

#include "Renderer.hpp"
#include "Tank.hpp"
#include "TankSpatialHash.hpp"
#include "Rocket.hpp"

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Tick();
private:
	const static int framesUntilShootCooldown = 200;

	Renderer* renderer;

	Tank tanksBlue[1279];
	Rocket rocketsBlue[5000];
	int rocketBlueCount = 0;

	Tank tanksRed[1279];
	Rocket rocketsRed[5000];
	int rocketRedCount = 0;

	TankSpatialHash* tshBlue;
	TankSpatialHash* tshRed;


	int framesUntilShoot = 1;

	void TickBlueTanks(int start, int count);
	void TickRedTanks(int start, int count);
	void TickShooting();
	void TickMoveRockets(int start, int count);
	void RenderBlueTanks(int start, int count);
	void RenderBlueRockets(int start, int count);
	void RenderRedTanks(int start, int count);
	void RenderRedRockets(int start, int count);
};