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
    cpu.SetBall(&ball);

    MenuOption choice = ShowMenu();
    switch(choice)
    {
        case MenuOption::OnePlayer:
            ball.SetPaddles({ &player1, &cpu });
            GameController::GetInstance()->Run({ &ball, &player1, &cpu });
            break;

        case MenuOption::TwoPlayers:
            ball.SetPaddles({ &player1, &player2 });
            GameController::GetInstance()->Run({ &ball, &player1, &player2 });
            break;

        case MenuOption::Exit:
            CloseWindow();
            return 0;
    }

    ShowWinnerScreen();

    CloseWindow();
    return 0;
}

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
