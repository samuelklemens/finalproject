#ifndef SCORE_H
#define SCORE_H
#include <iostream>

class Score
{
    double whiteScore = 0;
    double blackScore = 0;
    public:
        Score();
        double getWhiteScore() const;
        double getBlackScore() const;
        void changeWhiteScore(double num);
        void changeBlackScore(double num);
};

std::ostream &operator<<(std::ostream &out, const Score &s);
#endif