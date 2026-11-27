#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include "Position.h"
#include <vector>

class Queen : public Piece
{
public:
    Queen(char symbol, Position pos);
    ~Queen() override = default;

    std::vector<Position> validMoves(const Board &board) const override;
};

#endif