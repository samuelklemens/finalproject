#ifndef PAWN_H
#define PAWN_H
#include "Position.h"
#include "Piece.h"
#include <vector>

class Pawn: public Piece
{
    public: 
        //constructor and destructor
        Pawn(char symbol, Position pos);
        virtual ~Pawn();

        bool isFirstMove() const;
        bool canPromote() const;
        bool canEnpassant(const Board &board, bool rightSide) const;
        
        virtual std::vector<Position> attackedSquares(const Board &board) const override;

        virtual std::vector<Position> validMoves(const Board &board) const override;
        
};

#endif