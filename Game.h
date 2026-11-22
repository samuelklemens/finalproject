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
    bool whitesTurn = true;
    bool running = false;
    bool isSetup = false;
    std::string gameState = "";
    public:
        Game();
        ~Game();
        bool isRunning() const;
        bool newGame(std::string white, std::string black);
        Board &getBoard();
        void setup();
        bool move(std::string startPos, std::string endPos, std::string promotion);
        bool aiMove();
        std::string resign();
        //annoying functions to acces methods in Board because communication only has acces to game
        bool setupSquare(char piece, std::string pos);
        bool setupRemoveSquare(std::string pos);
        bool setupTurn(std::string colour);
        bool done();
        std::string getState() const;//its private to game so i need this to print
        std::string printBoard() const;
        std::string printFinalScore() const;
};

#endif