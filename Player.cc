#include "Player.h"

Player::Player(std::string colour) : colour{colour} {}

Player::~Player() {}

std::string Player::getColour() const
{
    return colour;
}