#ifndef HUMAN_H
#define HUMAN_H
#include "Player.h"
#include <string>

class Human: public Player
{
    public:
        Human(std::string colour);
        bool isAi() const override;
        //should return false
        std::string makeMove(Board *board) const override;
        //should just return "" nothing else this should never be called
};

#endif