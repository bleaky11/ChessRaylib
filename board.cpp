#include <string>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

/**
 * Using default piece to fill in empty board space
 * Use subclasses if there is a piece there
 */
class Piece{
    public:
        string color;
        /**currPos is the  */
        int currPos[2];
        virtual int calcMovement(){
            return 0;
        }
        vector<pair<int, int>> possibleMoves;

};

class pawn: public Piece{
    public:
        int calcMovement() override{
            int movement;
            if(this->color == "White"){
                movement = 1;
            }if(this->color == "Black"){
                movement = -1;
            }possibleMoves.push_back({this->currPos[0], this->currPos[1]+movement});
            possibleMoves.push_back({this->currPos[0]+1, this->currPos[1]+movement});
            possibleMoves.push_back({this->currPos[0]-1, this->currPos[1]+movement});
        }
};

class board{
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

