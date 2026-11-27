#include "Rook.h"
#include "Board.h"

Rook::Rook(char symbol, Position pos, bool moved) : Piece{symbol, pos}, moved{moved} {}

bool Rook::hasMoved() const
{
    return moved;
}

void Rook::setPos(Position newPos)
{
    Piece::setPos(newPos);
    moved = true;
}

std::vector<Position> Rook::validMoves(const Board &board) const
{
    std::vector<Position> moves;
    Position cur = getPos();

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    for (int d = 0; d < 4; ++d)
    {
        Position next{cur.x + dx[d], cur.y + dy[d]};
        while (next.validPos())
        {
            Piece *target = board.getSquare(next);
            if (target == nullptr)
            {
                moves.push_back(next);
            }
            else
            {
                if (target->isWhite() != isWhite())
                {
                    moves.push_back(next);
                }
                break;
            }
            next.x += dx[d];
            next.y += dy[d];
        }
    }

    return moves;
}