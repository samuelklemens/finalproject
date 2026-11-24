#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include "Position.h"

class Board; 

class Piece
{
    char symbol; //char that defines what type of piece this is. ex: K for white king, r for black rook
    Position pos;
    // made the position struct an object with x and y value
    //we can convert from int to char notation as needed since conversion is 1-1. ex x = 1 y = 0 -> b1

    public:

        //i will implement the big five if we need it but for now just writing the regular constructor and destructor
        Piece(char symbol, Position pos);
        virtual ~Piece(); 

        //returns the position of this piece
        Position getPos() const;
        //sets position
        virtual void setPos(Position newPos);
        //returns symbol
        char getSymbol() const;
        //returns if this piece is white, if false then its obviously black
        bool isWhite() const;

        //returns vector of all valid moves. vector knows its own size
        virtual std::vector<Position> validMoves(const Board &board) const;

        //returns vector of all attacked squares of the current piece
        virtual std::vector<Position> attackedSquares(const Board &board) const;

};


#endif