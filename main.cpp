#pragma once

#include "game.h"

class Paddle_Class
{
    protected:

    void LimitMovement()
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

    public:
    float x, y;
    float speed_y;
    float padl_width, padl_height;
    Color color;
    PlayerType type;

    void Draw() const
    {
        DrawRectangleRounded(Rectangle{x, y, padl_width, padl_height}, 0.8f, 0, color);
    }

    void Update()
    {
        //paddle movement
        if (type == PlayerType::Blue)
        {
            if(IsKeyDown(KEY_UP)) y = y - speed_y;
            if(IsKeyDown(KEY_DOWN)) y = y + speed_y;
        }
        else if (type == PlayerType::Red)
        {
            if(IsKeyDown(KEY_W)) y = y - speed_y;
            if(IsKeyDown(KEY_S)) y = y + speed_y;
        }

        //window border restriction
        LimitMovement();
    }

};

class CpuPaddle_Class: public Paddle_Class
{
    public:

    void Update(int ball_y)
    {
        //paddle movement
        if(y + padl_height/2 > ball_y)
        {
            y = y - speed_y;
        }
        if(y + padl_height/2 < ball_y)
        {
            y = y + speed_y;
        }

        //window border restriction
        LimitMovement();
    }
};

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

Paddle_Class player1; 
Paddle_Class player2;
CpuPaddle_Class cpu;

void PvE(Ball &ball, Paddle_Class &player1, CpuPaddle_Class &cpu);
void PvP(Ball &ball, Paddle_Class &player1, Paddle_Class &player2);
void ShowWinnerScreen();

int main() 
{
    InitWindow(window_width, window_height, game_name.c_str());
    SetTargetFPS(60);

    Ball ball((float)window_width / 2, (float)window_height / 2, 8, 8, 12, WHITE);

    player1.padl_width = 20;
    player1.padl_height = 100;
    player1.x = ((float)window_width - player1.padl_width - 20);
    player1.y = ((float)window_height / 2 - player1.padl_height / 2);
    player1.speed_y = 6;
    player1.color = BLUE;
    player1.type = PlayerType::Blue;

    player2.padl_width = 20;
    player2.padl_height = 100;
    player2.x = 20;
    player2.y = (float)window_height / 2 - player2.padl_height / 2;
    player2.speed_y = 6;
    player2.color = RED;
    player2.type = PlayerType::Red;

    cpu.padl_width = 20;
    cpu.padl_height = 100;
    cpu.x = 20;
    cpu.y = (float)window_height / 2 - cpu.padl_height / 2;
    cpu.speed_y = 6;
    cpu.color = RED;

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

void PvE(Ball &ball, Paddle_Class &player1, CpuPaddle_Class &cpu)
{
    //1.Update game objects positions.
    ball.Update();
    player1.Update();
    cpu.Update((int)ball.y);
        
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

void PvP(Ball &ball, Paddle_Class &player1, Paddle_Class &player2)
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
