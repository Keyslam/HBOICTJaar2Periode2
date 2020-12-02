#pragma once
#include "Tank.hpp"
#include <vector>
#include <functional>
#include <map>
typedef std::vector<Tank*> TankSpatialCell;

class TankSpatialHash
{
public:
	TankSpatialHash(int mapWidth, int mapHeight);
	~TankSpatialHash();

	void AddTank(Tank& tank);
	void RemoveTank(Tank& tank);
	void UpdateTank(Tank& tank);

	void EachOverlappingCell(int x, int y, int w, int h, const std::function<void(TankSpatialCell*)>& f);
private:
	int cellSize;

	int hashWidth;
	int hashHeight;

	std::vector<TankSpatialCell> cells;

	int PositionToCell(int x, int y);
};