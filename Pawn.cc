#include "Position.h"
#include "Pawn.h"
#include <vector>
#include "Board.h"
using namespace std;

Pawn::Pawn(char symbol, Position pos) : Piece{symbol, pos}
{

}

Pawn::~Pawn()
{

}

bool Pawn::isFirstMove() const
{
    int rank;
    if(isWhite())
    {
        rank = 1;
    }
    else
    {
        rank = 6;
    }

    if(getPos().y == rank)
    {
        return true;
    }
    return false;
}

bool Pawn::canPromote() const
{
    if(isWhite() && getPos().y == 7)
    {
        return true;
    }
    if(!isWhite() && getPos().y == 0)
    {
        return true;
    }
    return false;
}

vector<Position> Pawn::attackedSquares(const Board &board) const
{
    vector<Position> attacked;

    int direction = 1;
    if (!isWhite())
    {
        direction = -1;
    }

    if (Position{getPos().x + 1, getPos().y + direction}.validPos())
    {
        attacked.push_back(Position{getPos().x + 1, getPos().y + direction});
    }
    if (Position{getPos().x - 1, getPos().y + direction}.validPos())
    {
        attacked.push_back(Position{getPos().x - 1, getPos().y + direction});
    }

    return attacked;
}

bool Pawn::canEnpassant(const Board &board, bool rightSide) const
{
    int direction = 1;
    char enemy = 'p';
    if(!isWhite())
    {
        direction = -1;
        enemy = 'P';
    }

    Position A_sideDiagonal{getPos().x - 1, getPos().y + direction};
    Position H_sideDiagonal{getPos().x + 1, getPos().y + direction};
    Position A_side{getPos().x - 1, getPos().y};
    Position H_side{getPos().x + 1, getPos().y};
    if (rightSide)
    {
        if(board.getSquare(H_side) != nullptr)
        {
            if(board.getEnPassantTarget() == H_sideDiagonal && board.getSquare(H_side)->getSymbol() == enemy)
            {
                return true;
            }
        }
        
    }
    else 
    {
        if(board.getSquare(A_side) != nullptr)
        {
            if(board.getEnPassantTarget() == A_sideDiagonal && board.getSquare(A_side)->getSymbol() == enemy)
            {
                return true;
            }
        }   
    }
    return false;
}

vector<Position> Pawn::validMoves(const Board &board) const
{
    vector<Position> valid;
    int direction = 1;
    if(!isWhite())
    {
        direction = -1;
    }
    Position oneInFront{getPos().x, getPos().y + 1*direction};
    Position twoInFront{getPos().x, getPos().y + 2*direction};
    Position firstDiagonal{getPos().x - 1, getPos().y + direction};
    Position secondDiagonal{getPos().x + 1, getPos().y + direction};
    if(board.getSquare(oneInFront) == nullptr && oneInFront.validPos())
    {
        valid.push_back(oneInFront);
    }
    if(board.getSquare(oneInFront) == nullptr && board.getSquare(twoInFront) == nullptr && isFirstMove() && twoInFront.validPos())
    {
        valid.push_back(twoInFront);
    }
    if(canEnpassant(board, true))
    {
        valid.push_back(board.getEnPassantTarget());
    }
    if(canEnpassant(board, false))
    {
        valid.push_back(board.getEnPassantTarget());
    }
    if(board.getSquare(firstDiagonal) != nullptr)
    {
        if(board.getSquare(firstDiagonal)->isWhite() != isWhite() && firstDiagonal.validPos())
        {
            valid.push_back(firstDiagonal);
        }
    }
    if(board.getSquare(secondDiagonal) != nullptr)
    {
        if(board.getSquare(secondDiagonal)->isWhite() != isWhite() && secondDiagonal.validPos())
        {
            valid.push_back(secondDiagonal);
        }
    }
    
    
    return valid;
}