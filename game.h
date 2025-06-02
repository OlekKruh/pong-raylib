#include <raylib.h>
#pragma once

#include <vector>
#include <string>

const int window_width = 1280;
const int window_height = 800;
const std::string game_name = "PingPong Game";
const Color transparent_yellow = { 253, 249, 0, 20 };

const int max_score = 5;

enum class PlayerType { Blue, Red, Other };
enum class MenuOption { OnePlayer, TwoPlayers, Exit };

class GameController
{
	// private constructor allows creation from this class only
	GameController() {}

	// explicitly disable copying and assignment
	GameController(GameController&&) = delete;
	GameController(const GameController&) = delete;
	GameController& operator=(GameController&&) = delete;
	GameController& operator=(const GameController&) = delete;

public:
	// https://refactoring.guru/design-patterns/singleton/cpp/example#example-0
	static GameController* GetInstance();

	void IncreaseCpuScore();
	void IncreasePlayerScore();

	int GetCpuScore() const { return cpu_score; }
	int GetPlayerScore() const { return player_score; }

private:
	int player_score = 0;
	int cpu_score = 0;
};
