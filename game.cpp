#pragma once

#include "game.h"

static GameController* instance;

GameController* GameController::GetInstance()
{
    if (!instance) {
        instance = new GameController();
    }

    return instance;
}

void GameController::IncreaseCpuScore()
{
	++cpu_score;
}

void GameController::IncreasePlayerScore()
{
	++player_score;
}