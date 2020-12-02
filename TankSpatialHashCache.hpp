#pragma once
struct TankSpatialHashCache {
	int topLeftIndex;
	int bottomRightIndex;

	TankSpatialHashCache() : topLeftIndex(-1), bottomRightIndex(-1) { }
};