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
	const static int VIRTUAL_GAME_WIDTH = 1280;
	const static int VIRTUAL_GAME_HEIGHT = 1678;
	const static int TANK_SHOOT_COOLDOWN = 200;
	
	Renderer* renderer;

	Tank tanksBlue[1279];
	Rocket rocketsBlue[5116]; // Find distance from bottom left to top right point. Divide by rocket speed. Ceil(speed / shootDelay) * 1279. Maximum rocket count. 
	int rocketBlueCount = 0;

	Tank tanksRed[1279];
	Rocket rocketsRed[5116]; // Find distance from bottom left to top right point. Divide by rocket speed. Ceil(speed / shootDelay) * 1279. Maximum rocket count. 
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

	Tank& FindClosestRedTank(Vector2<float> position);
	Tank& FindClosestBlueTank(Vector2<float> position);
};