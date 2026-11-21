#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Score.h"
#include "Position.h"
#include <string>

Game::Game()
{
    board = new Board();
    score = new Score();
}

Game::~Game()
{
    delete whitePlayer;
    delete blackPlayer;
    delete board;
    delete score;
}

bool Game::isRunning() const//need this to know if you can do setup() or not
{
    return running;
}

bool Game::newGame(string white, string black)
{
    /*
    When game white-player black-player is called it has to delete the players as the player could be 
    changed to an ai or human but the score should be kept the same 
    */
    if (whitePlayer != nullptr && blackPlayer != nullptr)
    {
        delete whitePlayer;
        delete blackPlayer;
    }
    //set up white player yeah i know i could do this better but it works for now
    if (white == "human") {
        whitePlayer = new Human{"white", true};//im assuming players are: string colour, bool isTurn, int level if ai
    } else if (white == "computer1") {
        whitePlayer = new Computer{"white", true, 1};
    } else if (white == "computer2") {
        whitePlayer = new Computer{"white", true, 2};
    } else if (white == "computer3") {
        whitePlayer = new Computer{"white", true, 3};
    } else if (white == "computer4") {
        whitePlayer = new Computer{"white", true, 4};
    } else {
        return false;
    }
    //set up black
    if (black == "human") {
        blackPlayer = new Human{"black", false};
    } else if (black == "computer1") {
        blackPlayer = new Computer{"black", false, 1};
    } else if (black == "computer2") {
        blackPlayer = new Computer{"black", false, 2};
    } else if (black == "computer3") {
        blackPlayer = new Computer{"black", false, 3};
    } else if (black == "computer4") {
        blackPlayer = new Computer{"black", false, 4};
    } else {
        delete whitePlayer; // new game failed cant leave it here
        whitePlayer = nullptr;
        return false;
    }
    if (!isSetup)
    {
        board->defaultSetup();//make sure i dont reset board its setup
    }
    else
    {
        isSetup = false;
    }
    running = true;
    return true;
}

Board &Game::getBoard()
{
    return *board;
}

void Game::setup()
{
    isSetup = true;
    board->clear();
}

bool Game::move(string startPos, string endPos)
{

}

void Game::resign()
{

}

bool Game::setupSquare(char piece, string pos)
{
    string valid = "PNRBQKpnrbqk";
    Position p;
    if (pos.length() != 2) {return false;}
    p.x = pos[0] - 'a';//
    p.y = pos[1] - '1';//
    if (!p.validPos() || valid.find(piece) == string::npos)
    {
        return false;
    }
    else
    {
        board->setSquare(piece, p);
        return true;
    }
}

bool Game::setupRemoveSquare(string pos)
{
    Position p;
    if (pos.length() != 2) {return false;}
    p.x = pos[0] - 'a';//
    p.y = pos[1] - '1';//
    if (!p.validPos())
    {
        return false;
    }
    else
    {
        board->setSquare(' ', p);//make sure to have setSquare with "" remove the pos or make new func for rem
        return true;
    }
}
bool Game::setupTurn(string colour)
{
    //idk yet
}
bool Game::done()
{
    return board->isValidBoard();
}