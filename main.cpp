#include "raylib.h"
#include "board.cpp"

int DrawBoard(vector<pair<int, int>>);
int updateBoard(vector<vector<Piece*>>);

int main() {
    Board newBoard = Board();
    SetTraceLogLevel(LOG_WARNING);//Stops printing every file loaded into terminal
    InitWindow(800, 800, "raylib mac test");
    SetTargetFPS(30);
    Vector2 selectedSquare;
    bool waitingForInput = true;
    bool whiteTurn = true;//True is whites turn, false is black's turn
    bool potentialCheckmate = false;
    int coordx;
    int coordy; 
    string winner = "";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);    
        if(winner.empty()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                selectedSquare = GetMousePosition();
                coordx = static_cast<int>(selectedSquare.x/100);
                coordy = static_cast<int>(selectedSquare.y/100);
                if(coordx >= 0 && coordy >= 0){
                    //Attempts to move piece if possible first
                    if(newBoard.move(coordx, coordy) == -1){
                        potentialCheckmate = false;
                        if(whiteTurn){
                            if(newBoard.squares.at(coordy).at(coordx)->color != "black"){
                                newBoard.currSelected = newBoard.squares.at(coordy).at(coordx);
                            }else newBoard.currSelected = new Piece();
                        }else{
                            if(newBoard.squares.at(coordy).at(coordx)->color != "white"){
                                newBoard.currSelected = newBoard.squares.at(coordy).at(coordx);
                            }else newBoard.currSelected = new Piece();
                        }
                        if(newBoard.legalMoves(newBoard.currSelected)){//If theres a potential check among the moves, this returns true
                            potentialCheckmate = true;
                        }
                    } else{
                        if(whiteTurn && potentialCheckmate){
                            king* bk = dynamic_cast<king*>(newBoard.squares.at(newBoard.blackKing.second).at(newBoard.blackKing.first));
                            if(bk->inDanger(newBoard.squares, newBoard.blackKing.first, newBoard.blackKing.second)){
                                if(newBoard.checkMate(whiteTurn)){
                                    printf("White Player has won\n");
                                    winner = "white";
                                }
                            }
                        }else if(!whiteTurn && potentialCheckmate){
                            king* wk = dynamic_cast<king*>(newBoard.squares.at(newBoard.whiteKing.second).at(newBoard.whiteKing.first));
                            if(wk->inDanger(newBoard.squares, newBoard.whiteKing.first, newBoard.whiteKing.second)){
                                if(newBoard.checkMate(whiteTurn)){
                                    printf("Black Player has won\n");
                                    winner = "black";
                                }
                            }
                        }
                        newBoard.currSelected = new Piece();
                        whiteTurn = !whiteTurn;
                    }
                }
            }
        }
        DrawBoard(newBoard.possibleMoves);
        updateBoard(newBoard.squares);
        if(winner == "white"){
            DrawRectangle(125, 375, 550, 50, BLACK);
            DrawText("White Player wins!", 200, 375, 50, WHITE);
        }else if(winner == "black"){
            DrawRectangle(125, 375, 550, 50, RAYWHITE);
            DrawText("Black Player wins!", 200, 375, 50, BLACK);
        } 
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
                    DrawTexture(Pawn, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "bishop"){
                    DrawTexture(Bishop, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "queen"){
                    DrawTexture(Queen, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "king"){
                    DrawTexture(King, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "knight"){
                    DrawTexture(Knight, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }else if(squares.at(i).at(j)->pieceType == "rook"){
                    DrawTexture(Rook, 10 +  (j * 100), 10 + (i * 100), BLACK);
                }
            }else if(squares.at(i).at(j)->color == "white"){
                if(squares.at(i).at(j)->pieceType == "pawn"){
                    DrawTexture(Pawn, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "bishop"){
                    DrawTexture(Bishop, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "queen"){
                    DrawTexture(Queen, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "king"){
                    DrawTexture(King, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "knight"){
                    DrawTexture(Knight, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }else if(squares.at(i).at(j)->pieceType == "rook"){
                    DrawTexture(Rook, 10 +  (j * 100), 10 + (i * 100), BEIGE);
                }
            }
        }
    }
    return 0;
}

