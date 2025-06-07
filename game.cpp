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

void GameController::Run(const std::vector<IGameObject*> _objects)
{
    if (_objects.size() < 3) {
        //_ASSERT(_objects.size() < 3);
        return;
    }

    while (!WindowShouldClose() && GetPlayerScore() < max_score && GetCpuScore() < max_score) {

        for (IGameObject* obj : _objects) {
            if (obj)
                obj->Update();
        }

        ClearBackground(BLACK);
        BeginDrawing();

        DrawLine(window_width / 2, 0, window_width / 2, window_height, YELLOW);
        DrawCircle(window_width / 2, window_height / 2, 250, transparent_yellow);

        for (IGameObject* obj : _objects) {
            if (obj)
                obj->Draw();
        }

        DrawText(TextFormat("%i", GameController::GetInstance()->GetCpuScore()), window_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", GameController::GetInstance()->GetPlayerScore()), 3 * window_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) break;
    }
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

    for (Paddle* paddle : paddles)
    {
        if (CheckCollisionCircleRec(Vector2{ x, y }, radius,
            Rectangle{ paddle->x, paddle->y, paddle->padl_width, paddle->padl_height }))
        {
            speed_x *= -1;
        }
    }
}

void Ball::SetPaddles(const std::vector<Paddle*> _paddles)
{
    paddles = _paddles;
}

void Ball::ResetBall()
{
    x = (float)GetScreenWidth() / 2;
    y = (float)GetScreenHeight() / 2;

    int speed_choices[2] = { -1, 1 };
    speed_y *= speed_choices[GetRandomValue(0, 1)];
    speed_x *= speed_choices[GetRandomValue(0, 1)];
}

void Paddle::Draw() const
{
    DrawRectangleRounded(Rectangle{ x, y, padl_width, padl_height }, 0.8f, 0, color);
}

void Paddle::Update()
{
    if (type == PlayerType::Blue)
    {
        if (IsKeyDown(KEY_UP)) y = y - speed_y;
        if (IsKeyDown(KEY_DOWN)) y = y + speed_y;
    }
    else if (type == PlayerType::Red)
    {
        if (IsKeyDown(KEY_W)) y = y - speed_y;
        if (IsKeyDown(KEY_S)) y = y + speed_y;
    }

    //window border restriction
    LimitMovement();
}

void Paddle::LimitMovement()
{
    if (y <= 0)
    {
        y = 5;
    }
    if (y + padl_height >= GetScreenHeight())
    {
        y = GetScreenHeight() - padl_height - 5;
    }
}

void CpuPaddle::Update()
{
    if (!ball) {
        //_ASSERT(ball != nullptr);
        return;
    }

    if (y + padl_height / 2 > ball->y)
    {
        y = y - speed_y;
    }
    if (y + padl_height / 2 < ball->y)
    {
        y = y + speed_y;
    }

    //window border restriction
    LimitMovement();
}