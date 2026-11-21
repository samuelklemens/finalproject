#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <iostream>
#include <string>

class Game;//forward declaration since it needs reference to board as a parameter

class Communication
{
    Game* game;//bec forward declare
    public:
        Communication(Game* game);
        ~Communication();
        void communicate();
};

#endif