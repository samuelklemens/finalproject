#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <fstream>
#include <string>
#include "Position.h"

class Board; //forward declaration since piece needs reference to board as a parameter

class Piece
{
    Position pos;
    // made the position struct an object with x and y value
    //we can convert from int to char notation as needed since conversion is 1-1. ex x = 1 y = 0 -> b1
    char symbol; //char that defines what type of piece this is. ex: K for white king, r for black rook
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
        //returns new[] array ending in a terminal Position{-1, -1} (apparently this is called a sentinel) 
        //so that callers know when the list ends
        //alternative is making a MoveList struct with array and count of how many moves are in the array 
        Position *validMoves(const Board &board) const
        {
            //this is temporary. i don't like this implementation since it breaks encapsulation. ill think of a better solution
            Position *moves = new Position[1];
            moves[0] = Position{-1, -1};
            return moves;
        }

};


#endif