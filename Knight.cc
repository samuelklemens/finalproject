#include "Knight.h"
#include "Board.h"

Knight::Knight(char symbol, Position pos) : Piece{symbol, pos} {}

std::vector<Position> Knight::validMoves(const Board &board) const{
    Position cur = getPos();
    std::vector<Position> moves;
    int dx[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    for (int i = 0; i < 8; ++i){
        Position p{dx[i] + cur.x, dy[i] + cur.y};
        if (!p.validPos()) continue;
        Piece *target = board.getSquare(p);
        if (target == nullptr || target->isWhite() != isWhite()){
            moves.push_back(p);
        }
    }
    return moves;
}

        





