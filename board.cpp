#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <cmath>

using namespace std;


/**
 * Using default piece to fill in empty board space
 * Use subclasses if there is a piece there
 */
class Piece{
    public:
        /**
         * White 
         * Black
         * Empty
         */
        string color;
        int currPos[2];
        /**To calc all spots that this piece could move regardless of other board condition
         * 
         * Check isLegalMove later on to eliminate moves blocked by pieces, or if move endangers own king
         */
        virtual int calcMovement(){
            return 0;
        }
        vector<pair<int, int>> possibleMoves;
        /**Starts from 0, goes left - right, up - down
         * 0, 7, 56, 63 == rook
         * 1, 6, 57, 62 == knights
         * 2, 5, 58, 61 == bishops
         * 3, 59 == queen
         * 4, 60 == king
         * 
        */
        int pieceID;
        string pieceType;
};

class pawn: public Piece{//Add enpassant and promotion
    public:
        /**Check later on if enemy piece exists diagonally to filter out diagonal moves */
        int calcMovement() override{
            int movement;
            if(this->color == "White"){
                movement = -1;
            }if(this->color == "Black"){
                movement = 1;
            }possibleMoves.push_back({this->currPos[0], this->currPos[1]+movement});
            possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+movement});
            possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+movement});
            //Add a check for enpassant later
        }
};

class king: public Piece{
    public:
        int calcMovement() override{
            for(int horizonal = -1; horizonal <= 1; horizonal++){
                for(int vertical = 1; vertical <= 1; vertical++){
                    if(horizonal != 0 && vertical != 0){
                        possibleMoves.push_back({this->currPos[0]+(vertical), this->currPos[1]+(horizonal)});
                    }
                }   
            }
        }
        bool inDanger(Board boardState, int posX, int posY){
            //To know which direction enemy pawns attack from 
            //White is negative because black pawns attack from above, going positive means down
            //from king it would mean black pawn is above you, which means negative y direction
            int moveX = 1;
            int moveY = 1;
            if(bishopCheck(boardState, posX, posY, moveX, moveY))
                return true;
                
            return false;
        }
        bool bishopCheck(Board boardState, int posX, int posY, int moveX, int moveY){
            int tempX = posX;
            int tempY = posY;
            for(int i = 0; i < 8; i++){//Cut off early if encounters edge of board or other pieces
                tempX+=moveX; tempY+=moveY;
                if(tempX >= 8 || tempY >= 8){break;}//Edge of board, check other direction
                if(boardState.squares.at(tempX).at(tempY).color == this->color){break;}//Blocked by ally check other direction
                else if(boardState.squares.at(tempX).at(tempY).color == "Empty"){
                    continue;
                }else{
                    //PawnCheck
                    if(boardState.squares.at(tempX).at(tempY).pieceType == "pawn"){
                        if(tempY - posY == 1 && this->color == "Black" || tempY - posY == -1 && this->color == "White")
                            return true;
                        else break;
                    }//Queen and bishop check
                    else if(boardState.squares.at(tempX).at(tempY).pieceType == "queen" || boardState.squares.at(tempX).at(tempY).pieceType == "bishop"){
                        return true;
                    }
                }
            }
            return false;
        }
    };

/**
 * Idea is to make a 8x8 matrix, each spot containing either a real piece or a blank piece
 * 
 * x coordinate works normally, y coordinate goes down instead of up to align with raylib
 */
class Board{
    public:
        vector<vector<Piece>> squares;
        void move(int startx, int starty, int endx, int endy){
            Piece startPiece = squares.at(startx).at(starty);
            for(int i = 0; i < startPiece.possibleMoves.size(); i++){
                if(startPiece.possibleMoves.at(i) == make_pair(startx, starty)){
                    
                    break;
                }
            }
        };
        bool legalMove(int startx, int starty, int endx, int endy){

            return false;
        }
};

