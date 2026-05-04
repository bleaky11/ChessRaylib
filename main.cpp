#include "raylib.h"

int DrawBoard();

int main() {
    InitWindow(800, 800, "raylib mac test");
    SetTargetFPS(30);
    Texture2D Rook = LoadTexture("./Assets/Rook.png");
    Texture2D Knight = LoadTexture("./Assets/Knight.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawBoard();
        ClearBackground(RAYWHITE);
        DrawTexture(Rook, 10, 710, BLACK);
        DrawTexture(Knight, 110, 710, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int DrawBoard(){
    bool slot = true;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(slot)
                DrawRectangle(700 - (i * 100), 0 + (j * 100), 100, 100, BROWN);
            slot = !slot;
        }
        slot = !slot;
    }
    return 0;
}

