#ifndef SCORE_H
#define SCORE_H

class Score
{
    double whiteScore = 0;
    double blackScore = 0;
    public:
        Score();
        double getWhiteScore();
        double getBlackScore();
        void changeWhiteScore(double num);
        void changeBlackScore(double num);
};
#endif