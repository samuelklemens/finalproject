#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include "Piece.h"
#include "Position.h"

class Board
{
    Piece *squares[8][8]; //Board owns pieces, nullptr = empty square
    //this is an array of pointers, each pointing to a piece object on the heap
    Position EnPassantTarget;
    public:
        //regular constructor
        Board();

        //big five
        virtual ~Board();
        Board(const Board &other);
        Board(Board &&other);
        Board &operator=(const Board &other);
        Board &operator=(Board &&other);
        
        void setSquare(char piece, Position pos);
        Piece *getSquare(Position pos) const; //returns the piece at a given position, nullptr if nothing is there
        void movePiece(Position from, Position to); // deletes a captured piece

        Position getEnPassantTarget() const; //return position of en-passant target. position where the pawn enpassant-ing will be.
        //return position of {-1,-1} if none
        //in pawn, canEnpassant will check if this position is diagonally 1 to the left or right of it(ie it can enpassant)
        //This refreshes

        //sets up board with default pieces and positions
        /*
        8 rnbqkbnr
        7 pppppppp
        6  _ _ _ _
        5 _ _ _ _
        4  _ _ _ _
        3 _ _ _ _
        2 PPPPPPPP
        1 RNBQKBNR */
        void defaultSetup();
        
        void printBoard() const; 

        bool isValidBoard() const; //checks if the board is valid or not. ie: one king per side

        void clear();

        //returns true if the passed in position is attacked by an enemy piece
        bool isSquareAttacked(Position pos, bool byWhite) const;

        bool isInCheck(bool whiteKing) const;
        /*
        Depending on whos turn it is with the bool check if their king is attacked by any piece 
        and return true if it is
        */

        bool hasLegalMoves(bool whitesTurn) const;
        /*
        Checks every piece for either white or black depending on the bool and checks if there are any
        moves it can make that would not lead to check.
        */


};

std::ostream &operator<<(std::ostream &out, const Board &b);

#endif