#include <iostream>
#include <SDL.h>
#include <windows.h>

#include "Vector2.hpp"
#include "Game.hpp"
#include "Instrumentor.hpp"
#include "TankSpatialHash.hpp"

#include <chrono>

int main(int argc, char* argv[]) {
	// SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	// SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	PROFILE_START("Session");

	Game* game = new Game();

	bool success = game->Init();
	
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	{
		PROFILE_SCOPE("Logic");
		if (success) {
			for (int i = 0; i < 2000; i++) {
				game->Tick();
			}
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	auto difference = end - start;
	std::cout << difference.count() << " us" << std::endl;
	std::cin.get();

	PROFILE_END();

	return 0;
}