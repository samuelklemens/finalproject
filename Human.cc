#include "Human.h"

Human::Human(std::string colour) : Player{colour} {}

bool Human::isAi() const
{
    return false;
}

std::string Human::makeMove(Board * board) const
{
    return "";
}