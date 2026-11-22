#ifndef COMPUTER_H
#define COMPUTER_H
#include "Player.h"
#include <string>

class Computer: public Player
{
    int level;
    public:
        Computer(std::string colour, int level);
        bool isAi() const override;
        //should return true
        std::string makeMove(Board *board) const override;
        /*
        This funciton should return a string representing a move input eg: e2 e4, e7 e8 Q are two examples the moves returned
        are based on the level of the Computer
        level = 1: random legal moves
        level = 2: prefers capturing moves and checks over other moves
        level = 3: prefers avoiding capture, capturing moves, and checks
        level = 4: something more sophisticated... good luck!
        use the board methods for this from the given board also you could use sstream to make the move string to return 
        */
};

#endif