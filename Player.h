#ifndef PLAYER_H
#define PLAYER_H
#include <string>

class Board;

class Player
{
    std::string colour;
    public:
        Player(std::string colour);
        virtual ~Player();
        std::string getColour() const;
        virtual bool isAi() const = 0;
        virtual std::string makeMove(Board *board) const = 0;
};

#endif