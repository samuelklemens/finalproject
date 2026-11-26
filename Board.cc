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

//checks if the board is valid or not. ie: one king per side and no pawns in back rank, and neither king is in check
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

    if(blackKings != 1 || whiteKings != 1)
    {
        return false;
    }
    
    //once we know that theres only one king per side, check that those kings aren't in check
    return !isInCheck(true) && !isInCheck(false);
}

Position Board::getEnPassantTarget() const
{
    return EnPassantTarget;
}

//sets up board with default pieces and positions
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

//prints the board using overloaded output operator
void Board::printBoard() const
{
    cout << *this;
}

//executes move, is pseudo legal
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

    int rank = 7;
    if(p->isWhite())
    {
        rank = 0;
    }

    bool isPawn = (p->getSymbol() == 'P' || p->getSymbol() == 'p');
    bool isKing = (p->getSymbol() == 'K' || p->getSymbol() == 'k');
    
    //if enpassant, need to delete the piece beside the pawn that is executing the enpassant
    if(isPawn && getSquare(to) == nullptr && (from.x - to.x != 0) && to == EnPassantTarget)
    {
        delete squares[from.y][to.x];
        squares[from.y][to.x] = nullptr;
    }
    else if (isKing && (from.x - to.x == 2 || from.x - to.x == -2)) //if castled
    {
        if(to.x == 6 && squares[rank][7] != nullptr)//kingside castle, move kingside rook
        {
            squares[rank][5] = squares[rank][7];
            squares[rank][7] = nullptr;
            squares[rank][5]->setPos(Position{5, rank});
        }
        else if(to.x == 2 && squares[rank][0] != nullptr) //queenside castle, move queenside rook
        {
            squares[rank][3] = getSquare(Position{0, rank});
            squares[rank][0] = nullptr;
            squares[rank][3]->setPos(Position{3, rank});
        }
    }

    //make the move
    delete squares[to.y][to.x];
    squares[to.y][to.x] = p;
    squares[from.y][from.x] = nullptr;
    p->setPos(to);
    
    
    //if two square pawn move, then update enpassant target
    if(isPawn && ((from.y - to.y) == 2 || from.y - to.y == -2) )
    {
        EnPassantTarget = Position{from.x, (from.y + to.y) / 2};
    }
    else
    {
        EnPassantTarget = Position{-1,-1};
    }
    
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

//checks that a move doesn't leave one's own king in check
bool Board::isLegalMove(Position from, Position to, bool whitesTurn) const
{
    if(getSquare(from) == nullptr || !to.validPos() )//no piece to move or position isn't valid
    {
        return false;
    }

    if(getSquare(from)->isWhite() == whitesTurn)
    {
        Board copy(*this);
        copy.movePiece(from, to);
        //if the king isn't in check after checking a move, then its legal
        if(!(copy.isInCheck(whitesTurn))) 
        {
            return true;
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
                        if(isLegalMove(squares[i][j]->getPos(), p, whitesTurn))
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
