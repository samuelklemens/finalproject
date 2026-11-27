#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include "Position.h"
#include <vector>

class Rook : public Piece
{
    bool moved;

public:
    Rook(char symbol, Position pos, bool moved = false);
    ~Rook() override = default;

    bool hasMoved() const;
    void setPos(Position newPos) override;
    std::vector<Position> validMoves(const Board &board) const override;
};

#endif