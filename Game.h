#ifndef GAME_H
#define GAME_H
#include <string>

class Player;
class Board;
class Score;

class Game
{
    Player *whitePlayer = nullptr;
    Player *blackPlayer = nullptr;
    Board *board;
    Score *score;
    bool running = false;
    bool isSetup = false;
    public:
        Game();
        ~Game();
        bool isRunning() const;
        bool newGame(string white, string black);
        Board &getBoard();
        void setup();
        bool move(string startPos, string endPos);
        void resign();
        //annoying functions to acces methods in Board because communication only has acces to game
        bool setupSquare(char piece, string pos);
        bool setupRemoveSquare(string pos);
        bool setupTurn(string colour);
        bool done();
};
#endif