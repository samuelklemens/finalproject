#include "King.h"
#include <vector>
#include "Board.h"
#include "Rook.h"
using namespace std;

King::King(char symbol, Position pos, bool moved) : Piece{symbol, pos}, moved{moved}
{

}

King::~King()
{

}

bool King::isInCheck(const Board &board) const
{
    return board.isSquareAttacked(getPos(), !isWhite());
}

bool King::hasMoved() const
{
    return moved;
}

bool King::canCastle(const Board &board, bool kingside)const
{
    int rank;
    char rookColor;
    if (isWhite())
    {
        rank = 0;
        rookColor = 'R';
    }
    else
    {
        rank = 7;
        rookColor = 'r';
    }

    if(moved == true || getPos() != Position{4, rank} || isInCheck(board)) //check king is on home square and not moved, and isn't in check
    {
        return false;
    }
    
    if(kingside) 
    {
        //check that there is a rook of the same color in the queen or kingside corner
        if(board.getSquare(Position{7, rank}) == nullptr)
        {
            return false;
        }
        if(board.getSquare(Position{7, rank})->getSymbol() != rookColor) 
        {
            return false;
        }
         //and it hasn't moved
        if(static_cast<const Rook*>(board.getSquare(Position{7, rank}))->hasMoved())
        {
            return false;
        }
        //check space between rook and king is empty
        if(board.getSquare(Position{6, rank}) != nullptr || board.getSquare(Position{5, rank}) != nullptr)
        {
            return false;
        }
        // check the space between the king and rook isn't attacked
        if(board.isSquareAttacked(Position{6, rank}, !isWhite()) || board.isSquareAttacked(Position{5, rank}, !isWhite()))
        {
            return false;
        }
    }
    else
    {
        //check that there is a rook of the same color in the queen or kingside corner
        if(board.getSquare(Position{0, rank}) == nullptr)
        {
            return false;
        }
        if(board.getSquare(Position{0, rank})->getSymbol() != rookColor)
        {
            return false;
        }
        //and it hasn't moved
        if(static_cast<const Rook*>(board.getSquare(Position{0, rank}))->hasMoved())
        {
            return false;
        }
        //check space between rook and king is empty
        if(board.getSquare(Position{1, rank}) != nullptr || board.getSquare(Position{2, rank}) != nullptr || board.getSquare(Position{3, rank}) != nullptr)
        {
            return false;
        }
        // check the space between the king and rook isn't attacked
        if(board.isSquareAttacked(Position{2, rank}, !isWhite()) || board.isSquareAttacked(Position{3, rank}, !isWhite()))
        {
            return false;
        }
    }

    return true;

}

void King::setPos(Position newPos)
{
    Piece::setPos(newPos);
    moved = true;
}
        
vector<Position> King::validMoves(const Board &board) const
{
    vector<Position> valid;
    Position here = getPos();
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i == 0 && j == 0)//kings position. skip
            {
                continue;
            }
            Position p{here.x + i, here.y + j};
            if(p.validPos() && (board.getSquare(p) == nullptr || board.getSquare(p)->isWhite() != this->isWhite()))
            {
                    valid.push_back(p);
            }
        }
    }

    if(canCastle(board, true))
    {
        valid.push_back(Position{here.x + 2, here.y});
    }
    if(canCastle(board, false))
    {
        valid.push_back(Position{here.x - 2, here.y});
    }

    return valid;
}

vector<Position> King::attackedSquares(const Board &board) const
{
    vector<Position> attacked;
    Position here = getPos();

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i == 0 && j == 0)//kings position. skip
            {
                continue;
            }
            Position p{here.x + i, here.y + j};
            if(p.validPos())
            {
                attacked.push_back(p);
            }
        }
    }
    return attacked;
}