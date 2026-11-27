#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include "Position.h"
#include <vector>

class Knight : public Piece
{
public:
    Knight(char symbol, Position pos);
    ~Knight() override = default;

    std::vector<Position> validMoves(const Board &board) const override;
};

#endif