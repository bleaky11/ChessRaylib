#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <cmath>

using namespace std;

class Board;

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
        virtual void calcMovement(Board boardState){}
        vector<pair<int, int>> possibleMoves;
        string pieceType;
        Piece(int x, int y){
            currPos[0] = x;
            currPos[1] = y;
            color = "BLANK";
            pieceType = "BLANK";
        }
};

class pawn: public Piece{//Add enpassant and promotion
    public:
        /**Check later on if enemy piece exists diagonally to filter out diagonal moves */
        void calcMovement(Board boardState) override{
            int movement;
            if(this->color == "White"){
                movement = -1;
            }if(this->color == "Black"){
                movement = 1;
            }if(this->currPos[1]+movement >= 0 && this->currPos[1]+movement < 8){
                possibleMoves.push_back({this->currPos[0], this->currPos[1]+movement});
                if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 &&
                boardState.squares.at(this->currPos[0]).at(this->currPos[1]+movement).color != this->color &&
                boardState.squares.at(this->currPos[0]).at(this->currPos[1]+movement).pieceType != "BLANK")//If piece exists and is enemy
                    possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+movement});
                if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8&&
                boardState.squares.at(this->currPos[0]).at(this->currPos[1]+movement).color != this->color &&
                boardState.squares.at(this->currPos[0]).at(this->currPos[1]+movement).pieceType != "BLANK")
                    possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+movement});
            }
            //Add a check for enpassant later
        }
};

class king: public Piece{
    public:
        void calcMovement(Board boardState) override{
            for(int horizonal = -1; horizonal <= 1; horizonal++){
                for(int vertical = 1; vertical <= 1; vertical++){
                    if(horizonal != 0 && vertical != 0){
                        if(boardState.squares.at(this->currPos[0]+(horizonal)).at(this->currPos[1]+(vertical)).color != this->color)
                            possibleMoves.push_back({this->currPos[0]+(horizonal), this->currPos[1]+(vertical)});
                    }
                }   
            }
        }
        /**
         * posX: Current X position
         * posY: Current Y position
         * moveX: positive or negative X movement
         * moveY: positive or negative y movement
         */
        bool boardCheck(Board boardState, int posX, int posY, int moveX, int moveY){
            int tempX = posX;
            int tempY = posY;
            for(int i = 0; i < 8; i++){//Cut off early if encounters edge of board or other pieces
                tempX+=moveX; tempY+=moveY;
                if(tempX >= 8 || tempY >= 8){break;}//Edge of board, check other direction
                if(boardState.squares.at(tempX).at(tempY).color == this->color){break;}//Blocked by ally check other direction
                else if(boardState.squares.at(tempX).at(tempY).color == "Empty"){
                    continue;
                }else if(abs(moveX) == abs(moveY)){//Diagonal Check
                    //PawnCheck
                    if(boardState.squares.at(tempX).at(tempY).pieceType == "pawn"){
                        if(tempY - posY == 1 && this->color == "Black" || tempY - posY == -1 && this->color == "White")
                            return true;
                        else break;
                    }//Queen and bishop check
                    else if(boardState.squares.at(tempX).at(tempY).pieceType == "queen" || boardState.squares.at(tempX).at(tempY).pieceType == "bishop"){
                        return true;
                    }
                }else{//Cardinal Check
                    if(boardState.squares.at(tempX).at(tempY).pieceType == "queen" || boardState.squares.at(tempX).at(tempY).pieceType == "rook"){
                        return true;
                    }
                }
            }
            return false;
        }bool knightCheck(Board boardState, int posX, int posY){
            if(boardState.squares.at(posX + 2).at(posY + 1).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX + 1).at(posY + 2).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX - 2).at(posY + 1).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX - 1).at(posY + 2).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX + 2).at(posY - 1).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX + 1).at(posY - 2).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX - 2).at(posY - 1).pieceType == "knight"){
                return true;
            }else if(boardState.squares.at(posX - 1).at(posY - 2).pieceType == "knight"){
                return true;
            }return false;
        }
        bool inDanger(Board boardState, int posX, int posY){
            //To know which direction enemy pawns attack from 
            //White is negative because black pawns attack from above, going positive means down
            //from king it would mean black pawn is above you, which means negative y direction
            
            for(int i = 1; i >= -1; i--){
                for(int j = 1; j >= -1; j--){
                    if(!(i == 0 && j == 0)){
                        if(boardCheck(boardState, posX, posY, 1, 1))
                            return true;
                    }
                }
            }
            if(knightCheck(boardState, posX, posY)){
                return true;
            }
        return false;
        }
        
};
class bishop: public Piece{
    public:
        void calcMovement(Board boardState) override{
            bool pospos = true;
            bool posneg = true;
            bool negpos = true;
            bool negneg = true;
            for(int i = 0; i < 8; i++){
                bool atLeastOne = false;//Using this to break out if none of the directions work anymore(piece in middle of board)
                if(pospos && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        }pospos = false;
                    }
                }if(posneg && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        }posneg = false;
                    }
                }if(negpos && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        }negpos = false;
                    }
                }if(negneg && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        }negneg = false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
};

class rook: public Piece{
    public:
        void calcMovement(Board boardState){
            bool up = true;
            bool right = true;
            bool down = true;
            bool left = true;
            for(int i = 0; i < 8; i++){
                bool atLeastOne = false;//Using this to break out if none of the directions work anymore(piece in middle of board)
                if(right && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        }right=false;
                    }
                }if(down && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        }down=false;
                    }
                }if(left && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        }left=false;
                    }
                }if(this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        }up=false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
};

class queen: public Piece{
    public:
        void calcMovement(Board boardState){
            bool pospos = true;
            bool posneg = true;
            bool negpos = true;
            bool negneg = true;
            bool up = true;
            bool right = true;
            bool down = true;
            bool left = true;
            for(int i = 0; i < 8; i++){
                bool atLeastOne = false;//Using this to break out if none of the directions work anymore(piece in middle of board)
                if(pospos && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        }pospos = false;
                    }
                }if(posneg && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        }posneg = false;
                    }
                }if(negpos && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        }negpos = false;
                    }
                }if(negneg && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        }negneg = false;
                    }
                }if(right && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]+i).at(this->currPos[1]).color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        }right=false;
                    }
                }if(down && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]+i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]+i).color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        }down=false;
                    }
                }if(left && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]-i).at(this->currPos[1]).color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        }left=false;
                    }
                }if(this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]-i).pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.squares.at(this->currPos[0]).at(this->currPos[1]-i).color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        }up=false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
};

class knight: public Piece{
    public:
        void calcMovement(Board boardState){
            if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 && this->currPos[1]+2 >= 0 && this->currPos[1]+2 < 8 &&
            boardState.squares.at(this->currPos[0]+1).at(this->currPos[1]+2).color != this->color){
                possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+2});
            }if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8 && this->currPos[1]-2 >= 0 && this->currPos[1]-2 < 8 &&
            boardState.squares.at(this->currPos[0]-1).at(this->currPos[1]-2).color != this->color){
                possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]-2});
            }if(this->currPos[0]+2 >= 0 && this->currPos[0]+2 < 8 && this->currPos[1]+1 >= 0 && this->currPos[1]+1 < 8 &&
            boardState.squares.at(this->currPos[0]+2).at(this->currPos[1]+1).color != this->color){
                possibleMoves.push_back({this->currPos[0]+2, this->currPos[1]+1});
            }if(this->currPos[0]-2 >= 0 && this->currPos[0]-2 < 8 && this->currPos[1]-1 >= 0 && this->currPos[1]-1 < 8 &&
            boardState.squares.at(this->currPos[0]-2).at(this->currPos[1]-1).color != this->color){
                possibleMoves.push_back({this->currPos[0]-2, this->currPos[1]-1});
            }if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 && this->currPos[1]-2 >= 0 && this->currPos[1]-2 < 8 &&
            boardState.squares.at(this->currPos[0]+1).at(this->currPos[1]-2).color != this->color){
                possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]-2});
            }if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8 && this->currPos[1]+2 >= 0 && this->currPos[1]+2 < 8 &&
            boardState.squares.at(this->currPos[0]-1).at(this->currPos[1]+2).color != this->color){
                possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+2});
            }if(this->currPos[0]+2 >= 0 && this->currPos[0]+2 < 8 && this->currPos[1]-1 >= 0 && this->currPos[1]-1 < 8 &&
            boardState.squares.at(this->currPos[0]+2).at(this->currPos[1]-1).color != this->color){
                possibleMoves.push_back({this->currPos[0]+2, this->currPos[1]-1});
            }if(this->currPos[0]-2 >= 0 && this->currPos[0]-2 < 8 && this->currPos[1]+1 >= 0 && this->currPos[1]+1 < 8 &&
            boardState.squares.at(this->currPos[0]-2).at(this->currPos[1]+1).color != this->color){
                possibleMoves.push_back({this->currPos[0]-2, this->currPos[1]+1});
            }
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
        Board(){

        }
};

