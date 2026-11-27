#include "Queen.h"
#include "Board.h"

Queen::Queen(char symbol, Position pos) : Piece{symbol, pos} {}

std::vector<Position> Queen::validMoves(const Board &board) const
{
    std::vector<Position> moves;
    Position cur = getPos();

    const int dx[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    const int dy[8] = {0, 0, 1, -1, 1, -1, 1, -1};

    for (int d = 0; d < 8; ++d)
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