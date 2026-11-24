#include "Board.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

Position Board::getEnPassantTarget() const
{
    return EnPassantTarget;
}

//sets up board with default pieces and positions
/*
8 rnbqkbnr
7 pppppppp
6  _ _ _ _
5 _ _ _ _
4  _ _ _ _
3 _ _ _ _
2 PPPPPPPP
1 RNBQKBNR 
  abcdefgh */
void Board::defaultSetup()
{
    clear();
    const char whiteBackRank[] = "RNBQKBNR"; //iterate through these in loop, pass the char at j to setSquare
    const char blackBackRank[] = "rnbqkbnr";
    for(int j = 0; j < 8; j++)
    {
        setSquare(whiteBackRank[j], Position{j, 0});
        setSquare('P', Position{j, 1});
        setSquare('p', Position{j, 6});
        setSquare(blackBackRank[j], Position{j, 7});
    }
}

std::ostream &operator<<(std::ostream &out, const Board &b)
{
    for (int i = 7; i >= 0; i--)
    {
        out << i + 1 << " ";
        for(int j = 0; j < 8; j++)
        {
            if (b.getSquare(Position{j,i}) == nullptr)
            {
                if((i + j) % 2 == 0)
                {
                    out << '_';
                }
                else
                {
                    out << ' ';
                }
            }
            else
            {
                out << b.getSquare(Position{j,i})->getSymbol();
            }
        }
        out << '\n';
    }
    out << "\n  abcdefgh\n";

    return out;
}

void Board::printBoard() const
{
    cout << *this;
}

void Board::movePiece(Position from, Position to)
{

    if(!(from.validPos() && to.validPos()) || from == to) //valid position check
    {
        return;
    }

    Piece *p = getSquare(from);

    if(p == nullptr)
    {
        return;
    }

    EnPassantTarget = Position{-1,-1};

    delete squares[to.y][to.x];
    squares[to.y][to.x] = p;
    squares[from.y][from.x] = nullptr;
    p->setPos(to);
}

//returns true if the passed in position is attacked by an enemy piece
bool Board::isSquareAttacked(Position pos, bool byWhite) const
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(squares[i][j] != nullptr && squares[i][j]->isWhite() == byWhite)
            {
                for(Position attack : squares[i][j]->attackedSquares(*this))
                {
                    if(attack == pos)
                    {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

bool Board::isInCheck(bool whiteKing) const
{
    char king;
    if(whiteKing)
    {
        king = 'K'; 
    }
    else
    {
        king = 'k';
    }

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(squares[i][j] != nullptr)
            {
                if(squares[i][j]->getSymbol() == king)
                {
                    Position p{j,i};
                    return isSquareAttacked(p, !whiteKing);
                }
            }
            
        }
    }
    
    return false;
}

bool Board::hasLegalMoves(bool whitesTurn) const
{
    
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(squares[i][j] != nullptr)
            {
                if(squares[i][j]->isWhite() == whitesTurn)
                {
                    vector<Position> valid = squares[i][j]->validMoves(*this);
                    for(Position p : valid)
                    {
                        Board copy(*this);
                        copy.movePiece(squares[i][j]->getPos(), p);
                        //if the king isn't in check after checking a move. then it has at least one legal move to make
                        if(!(copy.isInCheck(whitesTurn))) 
                        {
                            return true;
                        }
                    }
                }
            }
            
        }
    }
    
    return false;
}
