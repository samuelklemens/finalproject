#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Piece.h"
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
        whitePlayer = new Human{"white"};//im assuming players are: string colour, int level if ai
    } else if (white == "computer1") {
        whitePlayer = new Computer{"white", 1};
    } else if (white == "computer2") {
        whitePlayer = new Computer{"white", 2};
    } else if (white == "computer3") {
        whitePlayer = new Computer{"white", 3};
    } else if (white == "computer4") {
        whitePlayer = new Computer{"white", 4};
    } else {
        return false;
    }
    //set up black
    if (black == "human") {
        blackPlayer = new Human{"black"};
    } else if (black == "computer1") {
        blackPlayer = new Computer{"black", 1};
    } else if (black == "computer2") {
        blackPlayer = new Computer{"black", 2};
    } else if (black == "computer3") {
        blackPlayer = new Computer{"black", 3};
    } else if (black == "computer4") {
        blackPlayer = new Computer{"black", 4};
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

bool Game::move(string startPos, string endPos, string promotion)
{
    string valid = "NRBQnrbqk";
    Position ep, sp;
    if (startPos.length() != 2 || endPos.length() != 2) {return false; }//check valid position command
    ep.x = endPos[0] - 'a';
    ep.y = endPos[1] - '1';
    sp.x = startPos[0] - 'a';
    sp.y = startPos[1] - '1';
    if (valid.find(promotion) == string::npos && promotion != " ") {return false;}//check valid promotion command 
    if (!ep.validPos() || !sp.validPos()) {return false; }//check valid positions
    Piece* piece = board->getSquare(sp);
    if (piece == nullptr) {return false; }
    if ((piece->getSymbol(sp) != "p" && piece->getSymbol(sp) != "P") && promotion != " ")
    { return false; } //check if promoting somethign that is not a pawn

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
    if (colour == "white") {
        whitesTurn = true;
        return true;
    } else if (colour == "black") {
        whitesTurn = false;
        return true;
    } else{
        return false;
    }
}
bool Game::done()
{
    return board->isValidBoard();
}