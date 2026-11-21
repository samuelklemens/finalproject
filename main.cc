#include "Game.h"
#include "Communication.h"

int main()
{
    Game game;
    Communication com(&game);
    com.communicate();
}