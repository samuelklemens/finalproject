#include "Piece.h"
#include "Position.h"
#include <vector>
using namespace std;

Piece::Piece(char symbol, Position pos) : symbol{symbol}, pos{pos}
{

}

Piece::~Piece()
{

}

Position Piece::getPos() const
{
    return pos;
}

void Piece::setPos(Position newPos)
{
    pos = newPos;
}

char Piece::getSymbol() const
{
    return symbol;
}

bool Piece::isWhite() const
{
    return(symbol >= 'A' && symbol <= 'Z');
}

vector<Position> Piece::validMoves(const Board &board) const
{
    return vector<Position>(); //blank by default. override for each piece subtype (king pawn rook bishop queen rook knight)
}

vector<Position> Piece::attackedSquares(const Board &board) const
{
    return validMoves(board); 
    //default returns the valid moves, 
    //king and pawn override because their attack is different than their valid moves
}