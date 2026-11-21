#ifndef KING_H
#define KING_H
#include "Position.h"
#include "Piece.h"
#include <vector>

class King: public Piece
{
    bool moved; //has moved flag used for castling

    public: 
        //constructor and destructor
        King(char symbol, Position pos, bool moved = false);
        virtual ~King();

        bool isInCheck(const Board &board) const;
        bool canMove(const Board &board) const;
        bool hasMoved() const;
        bool canCastle(const Board &board, bool kingside)const;

        virtual void setPos(Position newPos) override; //this overide sets the moved flag to true
        
        virtual std::vector<Position> validMoves(const Board &board) const override;
        
};

#endif