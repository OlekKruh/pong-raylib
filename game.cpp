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

void Ball::Draw() const
{
    DrawCircle((int)x, (int)y, radius, color);
}

void Ball::Update()
{
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }

    if (x + radius >= GetScreenWidth())
    {
        GameController::GetInstance()->IncreaseCpuScore();
        ResetBall();
    }
    if (x - radius <= 0)
    {
        GameController::GetInstance()->IncreasePlayerScore();
        ResetBall();
    }
}

void Ball::ResetBall()
{
    x = (float)GetScreenWidth() / 2;
    y = (float)GetScreenHeight() / 2;

    int speed_choices[2] = { -1, 1 };
    speed_y *= speed_choices[GetRandomValue(0, 1)];
    speed_x *= speed_choices[GetRandomValue(0, 1)];
}