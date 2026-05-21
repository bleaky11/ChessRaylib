#include "raylib.h"
#include "board.cpp"

int DrawBoard();
int updateBoard(vector<vector<Piece*>> squares);

int main() {
    Board newBoard = Board();
    newBoard.printBoard();
    InitWindow(800, 800, "raylib mac test");
    SetTargetFPS(30);
    

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawBoard();
        ClearBackground(RAYWHITE);     
        updateBoard(newBoard.squares);
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

int updateBoard(vector<vector<Piece*>> squares){
    Texture2D Rook = LoadTexture("./Assets/Rook.png");
    Texture2D Knight = LoadTexture("./Assets/Knight.png");
    Texture2D Pawn = LoadTexture("./Assets/Pawn.png");
    Texture2D Bishop = LoadTexture("./Assets/Bishop.png");
    Texture2D Queen = LoadTexture("./Assets/Queen.png");
    Texture2D King = LoadTexture("./Assets/King.png");
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(squares.at(i).at(j)->color == "black"){
                if(squares.at(i).at(j)->pieceType == "pawn"){
                    DrawTexture(Pawn, 710 - (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "bishop"){
                    DrawTexture(Bishop, 710 - (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "queen"){
                    DrawTexture(Queen, 710 - (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "king"){
                    DrawTexture(King, 710 - (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "knight"){
                    DrawTexture(Knight, 710 - (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "rook"){
                    DrawTexture(Rook, 710 - (j * 100), 10 + (i * 100), BLACK);
                }
            }else if(squares.at(i).at(j)->color == "white"){
                if(squares.at(i).at(j)->pieceType == "pawn"){
                    DrawTexture(Pawn, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "bishop"){
                    DrawTexture(Bishop, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "queen"){
                    DrawTexture(Queen, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "king"){
                    DrawTexture(King, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "knight"){
                    DrawTexture(Knight, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "rook"){
                    DrawTexture(Rook, 710 - (j * 100), 10 + (i * 100), BEIGE);
                }
            }
        }
    }
    return 0;
}

