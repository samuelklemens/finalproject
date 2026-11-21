#include "Board.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Piece.h"
#include "Position.h"
#include "King.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
using namespace std;

//regular constructor
Board::Board() : EnPassantTarget{-1,-1}
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            squares[i][j] = nullptr;
        }
    }
}

//copy constructor
Board::Board(const Board &other)
{
    EnPassantTarget = other.EnPassantTarget;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            squares[i][j] = nullptr;
            const Piece *piece = other.squares[i][j];
            if(piece == nullptr)
            {
                continue;
            }
            Position p{j,i};
            switch(piece->getSymbol())
            {
                case 'K': case 'k': squares[i][j] = new King{piece->getSymbol(),  p, static_cast<const King*>(piece)->hasMoved()};
                break;
                case 'Q': case 'q': squares[i][j] = new Queen{piece->getSymbol(), p};
                break;
                case 'R': case 'r': squares[i][j] = new Rook{piece->getSymbol(),  p, static_cast<const Rook*>(piece)->hasMoved()};
                break;
                case 'N': case 'n': squares[i][j] = new Knight{piece->getSymbol(), p};
                break;
                case 'B': case 'b': squares[i][j] = new Bishop{piece->getSymbol(), p};
                break;
                case 'P': case 'p': squares[i][j] = new Pawn{piece->getSymbol(), p};
                break;
            }
        }
    }
}

//clear the board
void Board::clear()
{

    EnPassantTarget = Position{-1, -1};

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            delete squares[i][j];
            squares[i][j] = nullptr;
        }
    }
}

//move constructor
Board::Board(Board &&other)
{
    EnPassantTarget = other.EnPassantTarget;
    other.EnPassantTarget = {-1,-1};
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            squares[i][j] = other.squares[i][j];
            other.squares[i][j] = nullptr;
        }
    }
    
}

//copy assignment operator
Board &Board::operator=(const Board &other)
{
    if(this == &other) //self assignment case
    {
        return *this;
    }

    EnPassantTarget = other.EnPassantTarget;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            delete squares[i][j];
            squares[i][j] = nullptr;
            const Piece *piece = other.squares[i][j];
            if(piece == nullptr)
            {
                continue;
            }
            Position p{j,i};
            switch(piece->getSymbol())
            {
                case 'K': case 'k': squares[i][j] = new King{piece->getSymbol(),  p, static_cast<const King*>(piece)->hasMoved()};
                break;
                case 'Q': case 'q': squares[i][j] = new Queen{piece->getSymbol(), p};
                break;
                case 'R': case 'r': squares[i][j] = new Rook{piece->getSymbol(),  p, static_cast<const Rook*>(piece)->hasMoved()};
                break;
                case 'N': case 'n': squares[i][j] = new Knight{piece->getSymbol(), p};
                break;
                case 'B': case 'b': squares[i][j] = new Bishop{piece->getSymbol(), p};
                break;
                case 'P': case 'p': squares[i][j] = new Pawn{piece->getSymbol(), p};
                break;
            }
        }
    }

    return *this;
}

//move assignment operator
Board &Board::operator=(Board &&other)
{
    if(this == &other) //self assignment case
    {
        return *this;
    }

    EnPassantTarget = other.EnPassantTarget;
    other.EnPassantTarget = {-1, -1};
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            delete squares[i][j];
            squares[i][j] = other.squares[i][j];
            other.squares[i][j] = nullptr;
        }
    }

    return *this;
}

//destructor
Board::~Board()
{
    clear();
}

void Board::setSquare(char piece, Position pos)
{

    if(!pos.validPos()) //check that the position is in the bounds
    {
        return;
    }

    if(piece == 'k' || piece == 'K')
    {
        King *king = new King(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = king; //pos is a struct. members are public by default
    }
    else if(piece == 'p' || piece == 'P')
    {
        Pawn *pawn = new Pawn(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = pawn;
    }
    else if(piece == 'n' || piece == 'N')
    {
        Knight *knight = new Knight(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = knight;
    }
    else if(piece == 'r' || piece == 'R')
    {
        Rook *rook = new Rook(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = rook;
    }
    else if(piece == 'q' || piece == 'Q')
    {
        Queen *queen = new Queen(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = queen;
    }
    else if(piece == 'b' || piece == 'B')
    {
        Bishop *bishop = new Bishop(piece, pos);
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = bishop;
    }
    else if(piece == ' ')
    {
        delete squares[pos.y][pos.x];
        squares[pos.y][pos.x] = nullptr;
    }
}

Piece *Board::getSquare(Position pos) const
{
    if(!pos.validPos()) //check that the position is in the bounds
    {
        return nullptr;
    }

    return squares[pos.y][pos.x];
}

//checks if the board is valid or not. ie: one king per side
bool Board::isValidBoard() const
{
    int blackKings = 0, whiteKings = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(squares[i][j] != nullptr)
            {
                if((squares[i][j]->getSymbol() == 'P' || squares[i][j]->getSymbol() == 'p') && (i == 0 || i == 7))
                {
                    return false;
                }
                if(squares[i][j]->getSymbol() == 'k')
                {
                    blackKings++;
                }
                else if(squares[i][j]->getSymbol() == 'K')
                {
                    whiteKings++;
                }
            }
            
        }
    }

    return (blackKings == 1 && whiteKings == 1);
}