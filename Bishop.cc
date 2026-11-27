#include "Bishop.h"
#include "Board.h"

Bishop::Bishop(char symbol, Position pos) : Piece{symbol, pos} {}

std::vector<Position> Bishop::validMoves(const Board &board) const{
    std::vector<Position> moves;
    Position cur = getPos();

    const int dx[4] = {1, 1, -1, -1};
    const int dy[4] = {1, -1, 1, -1};

    for (int d = 0; d < 4; ++d){
        Position p{cur.x + dx[d], cur.y + dy[d]};
        while(p.validPos()){
            Piece *target = getSquare(p);
            if(target){
                if(target->isWhite() != isWhite()){
                    moves.push_back(p);
                }
                break;
            }
            moves.push_back(p);
            p.x += dx[d];
            p.y += dy[d];
        }
    }
    return moves;
}
