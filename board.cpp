#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

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
        virtual void calcMovement(vector<vector<Piece*>> boardState){}
        vector<pair<int, int>> possibleMoves;
        string pieceType;
        Piece()
            : color("BLANK"),
            currPos{-1, 1},
            pieceType("BLANK")
        {}
        Piece(int x, int y) : Piece(){
            currPos[0] = x;
            currPos[1] = y;
        }
        Piece(int x, int y, string inColor, string inPieceType) : Piece(x, y){
            color = inColor;
            pieceType = inPieceType;
        }
};

class pawn: public Piece{//Add enpassant and promotion
    public:
        /**Check later on if enemy piece exists diagonally to filter out diagonal moves */
        void calcMovement(vector<vector<Piece*>> boardState) override{
            int movement;
            if(this->color == "White"){
                movement = -1;
            }if(this->color == "Black"){
                movement = 1;
            }if(this->currPos[1]+movement >= 0 && this->currPos[1]+movement < 8){
                possibleMoves.push_back({this->currPos[0], this->currPos[1]+movement});
                if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 &&
                boardState.at(this->currPos[0]).at(this->currPos[1]+movement)->color != this->color &&
                boardState.at(this->currPos[0]).at(this->currPos[1]+movement)->pieceType != "BLANK")//If piece exists and is enemy
                    possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+movement});
                if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8&&
                boardState.at(this->currPos[0]).at(this->currPos[1]+movement)->color != this->color &&
                boardState.at(this->currPos[0]).at(this->currPos[1]+movement)->pieceType != "BLANK")
                    possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+movement});
            }
            //Add a check for enpassant later
        }
        pawn(int posx, int posy, string color): Piece(posx, posy, color, "pawn"){}
};

class king: public Piece{
    public:
        void calcMovement(vector<vector<Piece*>> boardState) override{
            for(int horizonal = -1; horizonal <= 1; horizonal++){
                for(int vertical = 1; vertical <= 1; vertical++){
                    if(horizonal != 0 && vertical != 0){
                        if(boardState.at(this->currPos[0]+(horizonal)).at(this->currPos[1]+(vertical))->color != this->color)
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
        bool boardCheck(vector<vector<Piece*>> boardState, int posX, int posY, int moveX, int moveY){
            int tempX = posX;
            int tempY = posY;
            for(int i = 0; i < 8; i++){//Cut off early if encounters edge of board or other pieces
                tempX+=moveX; tempY+=moveY;
                if(tempX >= 8 || tempY >= 8){break;}//Edge of board, check other direction
                if(boardState.at(tempX).at(tempY)->color == this->color){break;}//Blocked by ally check other direction
                else if(boardState.at(tempX).at(tempY)->color == "Empty"){
                    continue;
                }else if(abs(moveX) == abs(moveY)){//Diagonal Check
                    //PawnCheck
                    if(boardState.at(tempX).at(tempY)->pieceType == "pawn"){
                        if(tempY - posY == 1 && this->color == "Black" || tempY - posY == -1 && this->color == "White")
                            return true;
                        else break;
                    }//Queen and bishop check
                    else if(boardState.at(tempX).at(tempY)->pieceType == "queen" || boardState.at(tempX).at(tempY)->pieceType == "bishop"){
                        return true;
                    }
                }else{//Cardinal Check
                    if(boardState.at(tempX).at(tempY)->pieceType == "queen" || boardState.at(tempX).at(tempY)->pieceType == "rook"){
                        return true;
                    }
                }
            }
            return false;
        }bool knightCheck(vector<vector<Piece*>> boardState, int posX, int posY){
            if(boardState.at(posX + 2).at(posY + 1)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX + 1).at(posY + 2)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX - 2).at(posY + 1)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX - 1).at(posY + 2)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX + 2).at(posY - 1)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX + 1).at(posY - 2)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX - 2).at(posY - 1)->pieceType == "knight"){
                return true;
            }else if(boardState.at(posX - 1).at(posY - 2)->pieceType == "knight"){
                return true;
            }return false;
        }
        bool inDanger(vector<vector<Piece*>> boardState, int posX, int posY){
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
        king(int posx, int posy, string color): Piece(posx, posy, color, "king"){}
        
};
class bishop: public Piece{
    public:
        void calcMovement(vector<vector<Piece*>> boardState) override{
            bool pospos = true;
            bool posneg = true;
            bool negpos = true;
            bool negneg = true;
            for(int i = 0; i < 8; i++){
                bool atLeastOne = false;//Using this to break out if none of the directions work anymore(piece in middle of board)
                if(pospos && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        }pospos = false;
                    }
                }if(posneg && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        }posneg = false;
                    }
                }if(negpos && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        }negpos = false;
                    }
                }if(negneg && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        }negneg = false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
        bishop(int posx, int posy, string color): Piece(posx, posy, color, "bishop"){}
};

class rook: public Piece{
    public:
        void calcMovement(vector<vector<Piece*>> boardState){
            bool up = true;
            bool right = true;
            bool down = true;
            bool left = true;
            for(int i = 0; i < 8; i++){
                bool atLeastOne = false;//Using this to break out if none of the directions work anymore(piece in middle of board)
                if(right && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0){
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1])->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1])->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        }right=false;
                    }
                }if(down && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.at(this->currPos[0]).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        }down=false;
                    }
                }if(left && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1])->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1])->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        }left=false;
                    }
                }if(this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        }up=false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
        rook(int posx, int posy, string color): Piece(posx, posy, color, "rook"){}
};

class queen: public Piece{
    public:
        void calcMovement(vector<vector<Piece*>> boardState){
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
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]+i});
                        }pospos = false;
                    }
                }if(posneg && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]-i});
                        }posneg = false;
                    }
                }if(negpos && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]+i});
                        }negpos = false;
                    }
                }if(negneg && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0 && this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]-i});
                        }negneg = false;
                    }
                }if(right && this->currPos[0]+i < 8 && this->currPos[0]+i >= 0){
                    if(boardState.at(this->currPos[0]+i).at(this->currPos[1])->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]+i).at(this->currPos[1])->color != this->color){
                            possibleMoves.push_back({this->currPos[0]+i, this->currPos[1]});
                        }right=false;
                    }
                }if(down && this->currPos[1]+i < 8 && this->currPos[1]+i >= 0){
                    if(boardState.at(this->currPos[0]).at(this->currPos[1]+i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]).at(this->currPos[1]+i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]+i});
                        }down=false;
                    }
                }if(left && this->currPos[0]-i < 8 && this->currPos[0]-i >= 0){
                    if(boardState.at(this->currPos[0]-i).at(this->currPos[1])->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]-i).at(this->currPos[1])->color != this->color){
                            possibleMoves.push_back({this->currPos[0]-i, this->currPos[1]});
                        }left=false;
                    }
                }if(this->currPos[1]-i < 8 && this->currPos[1]-i >= 0){
                    if(boardState.at(this->currPos[0]).at(this->currPos[1]-i)->pieceType == "BLANK"){
                        possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        atLeastOne = true;
                    }else{
                        if(boardState.at(this->currPos[0]).at(this->currPos[1]-i)->color != this->color){
                            possibleMoves.push_back({this->currPos[0], this->currPos[1]-i});
                        }up=false;
                    }
                }if(!atLeastOne){
                    return;
                }
            }
        }
        queen(int posx, int posy, string color): Piece(posx, posy, color, "queen"){}
};

class knight: public Piece{
    public:
        void calcMovement(vector<vector<Piece*>> boardState){
            if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 && this->currPos[1]+2 >= 0 && this->currPos[1]+2 < 8 &&
            boardState.at(this->currPos[0]+1).at(this->currPos[1]+2)->color != this->color){
                possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+2});
            }if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8 && this->currPos[1]-2 >= 0 && this->currPos[1]-2 < 8 &&
            boardState.at(this->currPos[0]-1).at(this->currPos[1]-2)->color != this->color){
                possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]-2});
            }if(this->currPos[0]+2 >= 0 && this->currPos[0]+2 < 8 && this->currPos[1]+1 >= 0 && this->currPos[1]+1 < 8 &&
            boardState.at(this->currPos[0]+2).at(this->currPos[1]+1)->color != this->color){
                possibleMoves.push_back({this->currPos[0]+2, this->currPos[1]+1});
            }if(this->currPos[0]-2 >= 0 && this->currPos[0]-2 < 8 && this->currPos[1]-1 >= 0 && this->currPos[1]-1 < 8 &&
            boardState.at(this->currPos[0]-2).at(this->currPos[1]-1)->color != this->color){
                possibleMoves.push_back({this->currPos[0]-2, this->currPos[1]-1});
            }if(this->currPos[0]+1 >= 0 && this->currPos[0]+1 < 8 && this->currPos[1]-2 >= 0 && this->currPos[1]-2 < 8 &&
            boardState.at(this->currPos[0]+1).at(this->currPos[1]-2)->color != this->color){
                possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]-2});
            }if(this->currPos[0]-1 >= 0 && this->currPos[0]-1 < 8 && this->currPos[1]+2 >= 0 && this->currPos[1]+2 < 8 &&
            boardState.at(this->currPos[0]-1).at(this->currPos[1]+2)->color != this->color){
                possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+2});
            }if(this->currPos[0]+2 >= 0 && this->currPos[0]+2 < 8 && this->currPos[1]-1 >= 0 && this->currPos[1]-1 < 8 &&
            boardState.at(this->currPos[0]+2).at(this->currPos[1]-1)->color != this->color){
                possibleMoves.push_back({this->currPos[0]+2, this->currPos[1]-1});
            }if(this->currPos[0]-2 >= 0 && this->currPos[0]-2 < 8 && this->currPos[1]+1 >= 0 && this->currPos[1]+1 < 8 &&
            boardState.at(this->currPos[0]-2).at(this->currPos[1]+1)->color != this->color){
                possibleMoves.push_back({this->currPos[0]-2, this->currPos[1]+1});
            }
        }
        knight(int posx, int posy, string color): Piece(posx, posy, color, "knight"){}
};

    

/**
 * Idea is to make a 8x8 matrix, each spot containing either a real piece or a blank piece
 * 
 * x coordinate works normally, y coordinate goes down instead of up to align with raylib
 */
class Board{
    public:
        pair<int, int> whiteKing;
        pair<int, int> blackKing;
        vector<vector<Piece*>> squares;
        /**
         * All possible moves that can be made by currSelected
         */
        vector<pair<int, int>> possibleMoves;
        /**
         * currently selected piece
         * possibleMoves corresponds to moves this piece can legally make
         */
        Piece* currSelected;
        /**
         * Checks if movement is legal(piece movement rules)
         * Simulates moving piece to square
         * Checks if king is in danger
         * If legal, added to possibleMoves
         * 
         */
        void legalMoves(){
            Piece startPiece = *currSelected;
            int startx = currSelected->currPos[0];
            int starty = currSelected->currPos[1];
            startPiece.calcMovement(squares);//Using pointers, so I might not need to dynamic cast
            for(int i = 0; i < startPiece.possibleMoves.size(); i++){
                int endx = startPiece.possibleMoves[i].first;
                int endy = startPiece.possibleMoves[i].second;
                bool wid = false;//White in danger
                bool bid = false;//Black in danger
                vector<vector<Piece*>> testSquares = squares;//Simulating the board if this piece moves
                testSquares.at(endx).at(endy) = &startPiece;
                testSquares.at(startx).at(starty) =  new Piece(startx, starty);
                if(testSquares.at(whiteKing.first).at(whiteKing.second)->pieceType == "king"){
                    king* wk = dynamic_cast<king*>(testSquares.at(whiteKing.first).at(whiteKing.second));
                    if(wk->inDanger(squares, wk->currPos[0], wk->currPos[1])) wid = true;
                }if(testSquares.at(blackKing.first).at(blackKing.second)->pieceType == "king"){
                    king* bk = dynamic_cast<king*>(testSquares.at(blackKing.first).at(blackKing.second));
                    if(bk->inDanger(squares, bk->currPos[0], bk->currPos[1])) bid = true;
                }if(startPiece.color == "white" && wid){
                    continue;
                }else if(startPiece.color == "black" && bid){
                    continue;
                }else{
                    this->possibleMoves.push_back(make_pair(endx, endy));
                }
            }
        };
        Piece move(int endx, int endy){
            if(find(possibleMoves.begin(), possibleMoves.end(), make_pair(endx, endy)) != possibleMoves.end()){
                Piece taken = *squares.at(endx).at(endy);
                squares.at(currSelected->currPos[0]).at(currSelected->currPos[1]) = new Piece(currSelected->currPos[0], currSelected->currPos[1]);
                squares.at(endx).at(endy) = currSelected;
                squares.at(endx).at(endy)->currPos[0] = endx;
                squares.at(endx).at(endy)->currPos[1] = endy;
                return taken;
            }return Piece();
        }
        Board(){
            whiteKing = make_pair(3, 7);
            blackKing = make_pair(3, 0);
            squares = vector<vector<Piece*>>(8, vector<Piece*>(8, nullptr));
            for(int i = 0; i < 8; i++){//Up down
                for(int j = 0; j < 8; j++){//Left right
                    //Default blank spots are BLANK
                    string color = "BLANK";
                    string pieceType = "BLANK";

                    if(i < 2) color = "black";
                    else if(i > 5) color = "white";

                    if(j == 0 || j == 7) pieceType = "rook";
                    else if(j == 1 || j == 6) pieceType = "knight";
                    else if(j == 2 || j == 5) pieceType = "bishop";
                    else if(j == 3) pieceType = "queen";
                    else pieceType = "king";
                    if(i == 1 || i == 6) pieceType = "pawn";

                    //wanted to use switch case but it does not work with strings in c++
                    if(color != "BLANK"){
                        if(pieceType == "king"){
                            Piece* newPiece =  new king(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }else if(pieceType == "queen"){
                            Piece* newPiece = new queen(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }else if(pieceType == "knight"){
                            Piece* newPiece = new knight(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }else if(pieceType == "bishop"){
                            Piece* newPiece = new bishop(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }else if(pieceType == "rook"){
                            Piece* newPiece = new rook(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }else{
                            Piece* newPiece = new pawn(j, i, color);
                            squares.at(i).at(j) = (newPiece);
                        }
                    }else{
                        Piece* newPiece = new Piece(j, i);
                        squares.at(i).at(j) = (newPiece);
                    }
                }
            }
        }
        void printBoard(){
            string board = "";
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    if(j < 7) board = (board + squares.at(i).at(j)->pieceType[0] + "\t");
                    else board = (board + squares.at(i).at(j)->pieceType[0] + "\n");
                }
            }
            printf("%s", board.c_str());
        }
};

