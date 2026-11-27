#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include "Position.h"
#include <vector>

class Bishop : public Piece
{
public:
    Bishop(char symbol, Position pos);
    ~Bishop() override = default;

    std::vector<Position> validMoves(const Board &board) const override;
};

#endif