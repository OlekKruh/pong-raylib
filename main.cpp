#include <iostream>
#include <raylib.h>


int main() {
    int monitor_width = 1280;
    int monitor_hight = 720;
    const char* text = "Raylib is working!";
    int font_size = 20;

    InitWindow(monitor_width, monitor_hight, "PingPong");

    int text_width = MeasureText(text, font_size);
    int text_height = font_size; 

    int text_x = (monitor_width - text_width) / 2;
    int text_y = (monitor_hight - text_height) / 2;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(text, text_x, text_y, font_size, RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
