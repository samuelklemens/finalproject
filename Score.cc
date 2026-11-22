#include "Score.h"
#include <iostream>
using namespace std;

Score::Score()
{

}

double Score::getWhiteScore() const
{
    return whiteScore;
}

double Score::getBlackScore() const
{
    return blackScore;
}

void Score::changeWhiteScore(double num)
{
    whiteScore += num;
}

void Score::changeBlackScore(double num)
{
    blackScore += num;
}

std::ostream &operator<<(std::ostream &out, const Score &s)
{
    out << "White: " << s.getWhiteScore() << "\n" << "Black :" << s.getBlackScore() << endl;
    return out;
}