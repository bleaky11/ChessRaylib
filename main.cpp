#include "raylib.h"

int initBoard();

int main() {
    InitWindow(640, 640, "raylib mac test");
    SetTargetFPS(30);
    Texture2D Rook = LoadTexture("Rook.png");
    Texture2D Knight = LoadTexture("Knight.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        initBoard();
        ClearBackground(RAYWHITE);
        DrawTexture(Rook, 0, 560, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int initBoard(){
    bool slot = true;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(slot)
                DrawRectangle(560 - (i * 80), 0 + (j * 80), 80, 80, BROWN);
            slot = !slot;
        }
        slot = !slot;
    }
    return 0;
}

