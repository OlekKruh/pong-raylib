#pragma once

#include "game.h"

MenuOption ShowMenu()
{
    std::vector<std::string> options = 
    {
        "1. One Player",
        "2. Two Players",
        "3. Exit"
    };

    int selected = 0;
    int optionsSize = static_cast<int>(options.size());

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_DOWN))
        {
            selected = (selected + 1) % optionsSize;
        }
        if (IsKeyPressed(KEY_UP))
        {
            selected = (selected - 1 + optionsSize) % optionsSize;
        }
        if (IsKeyPressed(KEY_ENTER))
        {
            return static_cast<MenuOption>(selected);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const int fontSize = 30;

        std::string title = "Select the game:";
        int titleHeight = fontSize;
        int lineSpacing = 40;

        int optionsCount = (int)options.size();

        int totalMenuHeight = titleHeight + 20 + optionsCount * lineSpacing;

        int startY = (window_height - totalMenuHeight) / 2;

        int titleWidth = MeasureText(title.c_str(), fontSize);
        int titleX = (window_width - titleWidth) / 2;
        int titleY = startY;
        DrawText(title.c_str(), titleX, titleY, fontSize, WHITE);

        for (int i = 0; i < optionsCount; i++)
        {
            int textWidth = MeasureText(options[i].c_str(), fontSize);
            int x = (window_width - textWidth) / 2;
            int y = startY + titleHeight + 20 + i * lineSpacing;

            Color color = (i == selected) ? GREEN : GRAY;
            DrawText(options[i].c_str(), x, y, fontSize, color);
        }

        EndDrawing();
    }

    return MenuOption::Exit;
}

void PvE(Ball &ball, Paddle &player1, CpuPaddle &cpu);
void PvP(Ball &ball, Paddle &player1, Paddle &player2);
void ShowWinnerScreen();

int main() 
{
    InitWindow(window_width, window_height, game_name.c_str());
    SetTargetFPS(60);

    Ball ball((float)window_width / 2, (float)window_height / 2, 8, 8, 12, WHITE);

    const std::pair<float, float> paddleSize{20.f, 100.f};

    Paddle player1(
        (float)(window_width - paddleSize.first - 20),
        (float)(window_height / 2 - paddleSize.second / 2),
        6,
        paddleSize.first,
        paddleSize.second,
        BLUE,
        PlayerType::Blue
    );

    Paddle player2(player1);
    player2.x = 20;
    player2.color = RED;
    player2.type = PlayerType::Red;

    CpuPaddle cpu(player2);

    MenuOption choice = ShowMenu();

    int playerScore = GameController::GetInstance()->GetPlayerScore();
    int cpuScore = GameController::GetInstance()->GetCpuScore();
    switch(choice)
    {
        case MenuOption::OnePlayer:
            while(!WindowShouldClose() && playerScore < max_score && cpuScore < max_score)
            {
                playerScore = GameController::GetInstance()->GetPlayerScore();
                cpuScore = GameController::GetInstance()->GetCpuScore();
                PvE(ball, player1, cpu);
                if (IsKeyPressed(KEY_ESCAPE)) break;
            }
            break;

        case MenuOption::TwoPlayers:
            while(!WindowShouldClose() && playerScore < max_score && cpuScore < max_score)
            {
                playerScore = GameController::GetInstance()->GetPlayerScore();
                cpuScore = GameController::GetInstance()->GetCpuScore();
                PvP(ball, player1, player2);
                if (IsKeyPressed(KEY_ESCAPE)) break;
            }
            break;

        case MenuOption::Exit:
            CloseWindow();
            return 0;
    }

    ShowWinnerScreen();

    CloseWindow();
    return 0;
}

void PvE(Ball &ball, Paddle &player1, CpuPaddle &cpu)
{
    //1.Update game objects positions.
    ball.Update();
    player1.Update();

    cpu.SetBallY((int)ball.y);
    cpu.Update();

    //2.Collision detection
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
        Rectangle{player1.x, player1.y, player1.padl_width, player1.padl_height}))
    {
        ball.speed_x *= -1;
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
        Rectangle{cpu.x, cpu.y, cpu.padl_width, cpu.padl_height}))
    {
        ball.speed_x *= -1;
    }

    //3.Drow game objects.
    ClearBackground(BLACK);
    BeginDrawing();

    //fiald decor
    DrawLine(window_width/2, 0, window_width/2, window_height, YELLOW);
    DrawCircle(window_width/2, window_height/2, 250, transparent_yellow);

    //elements ball, padls
    ball.Draw();
    player1.Draw();
    cpu.Draw();

    DrawText(TextFormat("%i", GameController::GetInstance()->GetCpuScore()) ,window_width/4 -20 ,20 ,80 ,WHITE);
    DrawText(TextFormat("%i", GameController::GetInstance()->GetPlayerScore()) , 3 * window_width/4 -20 ,20 ,80 ,WHITE);

    EndDrawing();
};

void PvP(Ball &ball, Paddle &player1, Paddle &player2)
{
    //1.Update game objects positions.
    ball.Update();
    player1.Update();
    player2.Update();
        
    //2.Collision detection
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
        Rectangle{player1.x, player1.y, player1.padl_width, player1.padl_height}))
    {
        ball.speed_x *= -1;
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
        Rectangle{player2.x, player2.y, player2.padl_width, player2.padl_height}))
    {
        ball.speed_x *= -1;
    }

    //3.Drow game objects.
    ClearBackground(BLACK);
    BeginDrawing();

    //fiald decor
    DrawLine(window_width/2, 0, window_width/2, window_height, YELLOW);
    DrawCircle(window_width/2, window_height/2, 250, transparent_yellow);

    //elements ball, padls
    ball.Draw();
    player1.Draw();
    player2.Draw();
    DrawText(TextFormat("%i", GameController::GetInstance()->GetCpuScore()) ,window_width/4 -20 ,20 ,80 ,WHITE);
    DrawText(TextFormat("%i", GameController::GetInstance()->GetPlayerScore()) , 3 * window_width/4 -20 ,20 ,80 ,WHITE);

    EndDrawing();
};

void ShowWinnerScreen()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        std::string winnerText;
        Color winnerColor;

        if (GameController::GetInstance()->GetPlayerScore() >= max_score)
        {
            winnerText = "Blue Wins!";
            winnerColor = BLUE;
        }
        else if (GameController::GetInstance()->GetCpuScore() >= max_score)
        {
            winnerText = "Red Wins!";
            winnerColor = RED;
        }
        else
        {
            winnerText = "Game Over";
            winnerColor = WHITE;
        }

        int fontSize = 50;
        int textWidth = MeasureText(winnerText.c_str(), fontSize);
        int textHeight = fontSize;

        int x = (window_width - textWidth) / 2;
        int y = (window_height - textHeight) / 2;

        DrawText(winnerText.c_str(), x, y, fontSize, winnerColor);

        std::string instructionText = "Press ENTER to Exit";
        int instructionFontSize = 20;
        int instrWidth = MeasureText(instructionText.c_str(), instructionFontSize);
        int instrX = (window_width - instrWidth) / 2;
        int instrY = y + textHeight + 40;

        DrawText(instructionText.c_str(), instrX, instrY, instructionFontSize, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
            break;
    }
}
