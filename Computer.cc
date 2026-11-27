#include "Computer.h"
#include "Board.h"
#include "Piece.h"
#include "Position.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>

struct Move
{
    Position from;
    Position to;
    char promotion;
};

int pieceValue(char sym)
{
    switch (sym)
    {
    case 'P': case 'p': return 100;
    case 'N': case 'n': return 320;
    case 'B': case 'b': return 330;
    case 'R': case 'r': return 500;
    case 'Q': case 'q': return 900;
    default: return 0;
    }
}

std::string posToStr(Position p)
{
    char col = 'a' + p.x;
    char row = '1' + p.y;
    std::string s = "";
    s += col;
    s += row;
    return s;
}

std::string moveToStr(const Move &m)
{
    std::ostringstream oss;
    oss << posToStr(m.from) << " " << posToStr(m.to);
    if (m.promotion != '\0')
    {
        oss << " " << m.promotion;
    }
    return oss.str();
}

Move pickRandom(const std::vector<Move> &moves)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(std::time(0));
        seeded = true;
    }
    int idx = std::rand() % moves.size();
    return moves[idx];
}

Board simulateMove(const Board &board, const Move &m)
{
    Board sim(board);
    sim.movePiece(m.from, m.to);
    if (m.promotion != '\0')
    {
        sim.setSquare(m.promotion, m.to);
    }
    return sim;
}

bool isSquareAttacked(const Board &board, Position target, bool byWhite)
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece *p = board.getSquare({x, y});
            if (p == nullptr || p->isWhite() != byWhite)
            {
                continue;
            }

            char sym = p->getSymbol();

            if (sym == 'P' || sym == 'p')
            {
                int dir = byWhite ? 1 : -1;
                if (std::abs(target.x - x) == 1 && target.y == y + dir)
                {
                    return true;
                }
                continue;
            }

            if (sym == 'K' || sym == 'k')
            {
                if (std::abs(target.x - x) <= 1 && std::abs(target.y - y) <= 1 &&
                    !(target.x == x && target.y == y))
                {
                    return true;
                }
                continue;
            }

            std::vector<Position> moves = p->validMoves(board);
            for (const Position &dest : moves)
            {
                if (dest == target)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isCaptureMove(const Board &board, const Move &m, bool amWhite)
{
    Piece *target = board.getSquare(m.to);
    if (target != nullptr && target->isWhite() != amWhite)
    {
        return true;
    }

    Piece *mover = board.getSquare(m.from);
    if (mover != nullptr && (mover->getSymbol() == 'P' || mover->getSymbol() == 'p'))
    {
        Position ep = board.getEnPassantTarget();
        if (ep.validPos() && m.to == ep)
        {
            return true;
        }
    }
    return false;
}

std::vector<Move> getAllLegalMoves(const Board &board, bool amWhite)
{
    std::vector<Move> legal;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Position from{x, y};
            Piece *p = board.getSquare(from);
            if (p == nullptr || p->isWhite() != amWhite)
            {
                continue;
            }

            std::vector<Position> rawMoves = p->validMoves(board);
            for (const Position &to : rawMoves)
            {
                char promo = '\0';
                if (p->getSymbol() == 'P' && to.y == 7)
                {
                    promo = 'Q';
                }
                else if (p->getSymbol() == 'p' && to.y == 0)
                {
                    promo = 'q';
                }

                Move cand{from, to, promo};
                Board sim = simulateMove(board, cand);
                if (!sim.isInCheck(amWhite))
                {
                    legal.push_back(cand);
                }
            }
        }
    }
    return legal;
}

int evaluateBoard(const Board &board, bool amWhite)
{
    int score = 0;
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece *p = board.getSquare({x, y});
            if (p == nullptr)
            {
                continue;
            }

            int val = pieceValue(p->getSymbol());

            if (x >= 3 && x <= 4 && y >= 3 && y <= 4)
            {
                val += 20;
            }
            else if (x >= 2 && x <= 5 && y >= 2 && y <= 5)
            {
                val += 10;
            }

            if (p->getSymbol() == 'P')
            {
                val += (y - 1) * 8;
            }
            else if (p->getSymbol() == 'p')
            {
                val += (6 - y) * 8;
            }

            if (p->isWhite() == amWhite)
            {
                score += val;
            }
            else
            {
                score -= val;
            }
        }
    }
    return score;
}

Computer::Computer(std::string colour, int level) : Player{colour}, level{level} {}

bool Computer::isAi() const
{
    return true;
}

std::string Computer::makeMove(Board *board) const
{
    if (board == nullptr)
    {
        return "";
    }

    std::string c = getColour();
    bool amWhite = (!c.empty() && (c[0] == 'w' || c[0] == 'W'));

    std::vector<Move> legalMoves = getAllLegalMoves(*board, amWhite);
    if (legalMoves.empty())
    {
        return "";
    }

    if (level <= 1)
    {
        return moveToStr(pickRandom(legalMoves));
    }

    if (level == 2)
    {
        std::vector<Move> preferred;
        for (const Move &m : legalMoves)
        {
            Board sim = simulateMove(*board, m);
            if (isCaptureMove(*board, m, amWhite) || sim.isInCheck(!amWhite))
            {
                preferred.push_back(m);
            }
        }
        if (!preferred.empty())
        {
            return moveToStr(pickRandom(preferred));
        }
        return moveToStr(pickRandom(legalMoves));
    }

    if (level == 3)
    {
        std::vector<Move> tier1;
        std::vector<Move> tier2;
        std::vector<Move> tier3;

        for (const Move &m : legalMoves)
        {
            bool wasAttacked = isSquareAttacked(*board, m.from, !amWhite);
            bool capture = isCaptureMove(*board, m, amWhite);

            Board sim = simulateMove(*board, m);
            bool givesCheck = sim.isInCheck(!amWhite);
            bool destSafe = !isSquareAttacked(sim, m.to, !amWhite);

            bool avoidsCapture = wasAttacked && destSafe;

            if (avoidsCapture || ((capture || givesCheck) && destSafe))
            {
                tier1.push_back(m);
            }
            else if (capture || givesCheck)
            {
                tier2.push_back(m);
            }
            else if (destSafe)
            {
                tier3.push_back(m);
            }
        }

        if (!tier1.empty()) return moveToStr(pickRandom(tier1));
        if (!tier2.empty()) return moveToStr(pickRandom(tier2));
        if (!tier3.empty()) return moveToStr(pickRandom(tier3));
        return moveToStr(pickRandom(legalMoves));
    }

    int bestScore = -1000000;
    std::vector<Move> bestMoves;

    for (const Move &m : legalMoves)
    {
        Board sim = simulateMove(*board, m);
        std::vector<Move> oppMoves = getAllLegalMoves(sim, !amWhite);

        int moveScore = 0;
        if (oppMoves.empty())
        {
            moveScore = sim.isInCheck(!amWhite) ? 100000 : 0;
        }
        else
        {
            int checkBonus = sim.isInCheck(!amWhite) ? 15 : 0;
            int worstForUs = 1000000;

            for (const Move &om : oppMoves)
            {
                Board sim2 = simulateMove(sim, om);
                int sc = evaluateBoard(sim2, amWhite);
                if (sim2.isInCheck(amWhite))
                {
                    sc -= 30;
                }
                if (sc < worstForUs)
                {
                    worstForUs = sc;
                }
                if (worstForUs + checkBonus < bestScore)
                {
                    break;
                }
            }
            moveScore = worstForUs + checkBonus;
        }

        if (moveScore > bestScore)
        {
            bestScore = moveScore;
            bestMoves.clear();
            bestMoves.push_back(m);
        }
        else if (moveScore == bestScore)
        {
            bestMoves.push_back(m);
        }
    }

    return moveToStr(pickRandom(bestMoves));
}