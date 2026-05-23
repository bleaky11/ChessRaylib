#include "raylib.h"
#include "board.cpp"

int DrawBoard(vector<pair<int, int>>);
int updateBoard(vector<vector<Piece*>>);

int main() {
    Board newBoard = Board();
    newBoard.printBoard();
    InitWindow(800, 800, "raylib mac test");
    SetTargetFPS(30);
    Vector2 selectedSquare;
    bool waitingForInput = true;
    bool whiteTurn = true;//True is whites turn, false is black's turn
    int coordx;
    int coordy;

    while (!WindowShouldClose()) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            selectedSquare = GetMousePosition();
            coordx = static_cast<int>(selectedSquare.x/100);
            coordy = static_cast<int>(selectedSquare.y/100);
            if(coordx >= 0 && coordy >= 0){
                printf("x: %d, y: %d\n\n", coordx, coordy);
                newBoard.currSelected = newBoard.squares.at(coordy).at(coordx);
                newBoard.legalMoves();
            }
        }
        BeginDrawing();
        DrawBoard(newBoard.possibleMoves);
        ClearBackground(RAYWHITE);     
        updateBoard(newBoard.squares);
        EndDrawing();
    }

    

    CloseWindow();
    return 0;
}

int DrawBoard(vector<pair<int, int>> possibleMoves){
    bool slot = true;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(slot)
                DrawRectangle(700 - (i * 100), 0 + (j * 100), 100, 100, BROWN);
            slot = !slot;
        }
        slot = !slot;
    }
    if(possibleMoves.size() > 0){
        for(int i = 0; i < possibleMoves.size(); i++){
            DrawRectangle(10 + (possibleMoves[i].first * 100), 10 + (possibleMoves[i].second * 100), 80, 80, RED);
        }
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

