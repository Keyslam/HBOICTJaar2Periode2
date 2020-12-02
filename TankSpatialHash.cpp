#include "TankSpatialHash.hpp"

#include <iostream>
#include <algorithm>

#include "TankSpatialHashCache.hpp"
#include "Instrumentor.hpp"

TankSpatialHash::TankSpatialHash(int mapWidth, int mapHeight) {
	this->cellSize = Tank::collisionRadius;

	hashWidth = mapWidth / cellSize + (mapWidth % cellSize != 0 ? 1 : 0);
	hashHeight = mapHeight / cellSize + (mapHeight % cellSize != 0 ? 1 : 0);

	long cellCount = hashWidth * hashHeight;

	cells = std::vector<std::vector<Tank*>>(cellCount);
	for (long i = 0; i < cellCount; i++) {
		cells[i] = std::vector<Tank*>();
	}
}

TankSpatialHash::~TankSpatialHash()
{

}

void TankSpatialHash::AddTank(Tank& tank)
{
	int topLeftIndex = PositionToCell(tank.position.x - Tank::collisionRadius, tank.position.y - Tank::collisionRadius);
	int bottomRightIndex = PositionToCell(tank.position.x + Tank::collisionRadius, tank.position.y + Tank::collisionRadius);

	TankSpatialHashCache& hashCache = tank.spatialHashCache;
	hashCache.topLeftIndex = topLeftIndex;
	hashCache.bottomRightIndex = bottomRightIndex;

	EachOverlappingCell(tank.position.x - Tank::collisionRadius, tank.position.y - Tank::collisionRadius, Tank::collisionRadius * 2, Tank::collisionRadius * 2, [&](TankSpatialCell* cell) {
		cell->push_back(&tank);
	});
}

void TankSpatialHash::RemoveTank(Tank& tank) {
	EachOverlappingCell(tank.position.x - Tank::collisionRadius, tank.position.y - Tank::collisionRadius, Tank::collisionRadius * 2, Tank::collisionRadius * 2, [&](TankSpatialCell* cell) {
		int index = -1;
		for (int i = 0; i < cell->size(); i++) {
			if (&tank == (*cell)[i]) {
				index = i;
				break;
			}
		}

		if (index == -1)
			return;

		cell->erase(cell->begin() + index);
	});
}

void TankSpatialHash::UpdateTank(Tank& tank)
{
	int topLeftIndex = PositionToCell(tank.position.x - Tank::collisionRadius, tank.position.y - Tank::collisionRadius);
	int bottomRightIndex = PositionToCell(tank.position.x + Tank::collisionRadius, tank.position.y + Tank::collisionRadius);

	TankSpatialHashCache& hashCache = tank.spatialHashCache;

	if (topLeftIndex != hashCache.topLeftIndex || bottomRightIndex != hashCache.bottomRightIndex) {
		RemoveTank(tank);
		AddTank(tank);

		hashCache.topLeftIndex = topLeftIndex;
		hashCache.bottomRightIndex = bottomRightIndex;
	}
}

void TankSpatialHash::EachOverlappingCell(int x, int y, int w, int h, const std::function<void(TankSpatialCell*)>& f)
{
	int topLeftCellX = std::clamp(x / cellSize, 0, this->hashWidth - 1);
	int topLeftCellY = std::clamp(y / cellSize, 0, this->hashHeight - 1);

	int bottomRightCellX = std::clamp((x + w) / cellSize, 0, this->hashWidth-1);
	int bottomRightCellY = std::clamp((y + h) / cellSize, 0, this->hashHeight-1);

	for (int cellX = topLeftCellX; cellX <= bottomRightCellX; cellX++) {
		for (int cellY = topLeftCellY; cellY <= bottomRightCellY; cellY++) {
			int index = cellX + cellY * hashWidth;
			f(&cells[index]);
		}
	}
}

int TankSpatialHash::PositionToCell(int x, int y) {
	int cellX = x / cellSize;
	int cellY = y / cellSize;

	int index = (cellX % hashWidth) + (cellY * hashWidth);

	return index;
}