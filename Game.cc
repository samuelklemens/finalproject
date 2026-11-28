#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Piece.h"
#include "Score.h"
#include "Position.h"
#include "Human.h"
#include "Computer.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

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
    
    //set up white player yeah i know i could do this better but it works for now
    Player *newWhitePlayer = nullptr;
    Player *newBlackPlayer = nullptr;
    if (white == "human") {
        newWhitePlayer = new Human{"white"};//im assuming players are: string colour, int level if ai
    } else if (white == "computer1") {
        newWhitePlayer = new Computer{"white", 1};
    } else if (white == "computer2") {
        newWhitePlayer = new Computer{"white", 2};
    } else if (white == "computer3") {
        newWhitePlayer = new Computer{"white", 3};
    } else if (white == "computer4") {
        newWhitePlayer = new Computer{"white", 4};
    } else {
        return false;
    }
    //set up black
    if (black == "human") {
        newBlackPlayer = new Human{"black"};
    } else if (black == "computer1") {
        newBlackPlayer = new Computer{"black", 1};
    } else if (black == "computer2") {
        newBlackPlayer = new Computer{"black", 2};
    } else if (black == "computer3") {
        newBlackPlayer = new Computer{"black", 3};
    } else if (black == "computer4") {
        newBlackPlayer = new Computer{"black", 4};
    } else {
        delete newWhitePlayer; // new game failed cant leave it here
        newWhitePlayer = nullptr;
        return false;
    }
    if (newWhitePlayer != nullptr && newBlackPlayer != nullptr)
    {
        delete whitePlayer;
        delete blackPlayer;
        whitePlayer = newWhitePlayer;
        blackPlayer = newBlackPlayer;
    }
    if (!isSetup)
    {
        whitesTurn = true;//fix here 3
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
    whitesTurn = true;//fix here 2
    isSetup = true;
    board->clear();
}

bool Game::move(string startPos, string endPos, string promotion)//holy code rot worst thing i wrote
{
    string valid = "NRBQnrbq";
    string bValid = "nrbq";
    string wValid = "NRBQ";
    gameState = "";
    Position ep, sp;
    if (startPos.length() != 2 || endPos.length() != 2) {return false; }//check valid position command
    if (promotion != " " && promotion.length() != 1) {return false; }//check if promotion is length of 1
    ep.x = endPos[0] - 'a';
    ep.y = endPos[1] - '1';
    sp.x = startPos[0] - 'a';
    sp.y = startPos[1] - '1';
    if (valid.find(promotion) == string::npos && promotion != " ") {return false;}//check valid promotion command 
    if (!ep.validPos() || !sp.validPos()) {return false; }//check valid positions
    Piece* piece = board->getSquare(sp);
    if (piece == nullptr) {return false; }
    if ((piece->getSymbol() != 'p' && piece->getSymbol() != 'P') && promotion != " ")
    { return false; } //check if promoting somethign that is not a pawn
    if (piece->isWhite() == true && ep.y != 7 && promotion != " ") {return false; }//to promove need to be in end
    if (piece->isWhite() != true && ep.y != 0 && promotion != " ") {return false; }
    if (piece->isWhite() != whitesTurn) {return false; }//check if moving when its not their turn
    if (whitesTurn && promotion != " " && bValid.find(promotion) != string::npos) {return false; }//check if promoting wrong colour
    if (!whitesTurn && promotion != " " && wValid.find(promotion) != string::npos) {return false; }
    vector<Position> validMov = piece->validMoves(*board);
    bool epInValidPos = false;
    for (Position pos : validMov)
    {
        if (ep == pos) {epInValidPos = true; }
    }
    if (epInValidPos == false) {return false; }
    if (piece->getSymbol() == 'P' && whitesTurn && ep.y == 7 && promotion == " ") {return false; }//check if you dont promote when moving pawn to end
    if (piece->getSymbol() == 'p' && !whitesTurn && ep.y == 0 && promotion == " ") {return false; }
    if (!board->isLegalMove(sp, ep, whitesTurn)) {return false; }// check if move puts king in check
    //now the move must be valid
    board->movePiece(sp, ep);
    if (promotion != " ") 
    {
        board->setSquare(promotion[0], ep);
    }
    whitesTurn = !whitesTurn;//change turn
    //set game state after a move
    bool check = board->isInCheck(whitesTurn);//returns a bool whether black or white is in check
    bool noLegal = !board->hasLegalMoves(whitesTurn);//retuns a bool wheter black or white has any legal moves
    if (check && noLegal && !whitesTurn) {
        gameState = "Checkmate! White wins!";
        running = false;
        score->changeWhiteScore(1);
    } else if (check && noLegal && whitesTurn) {
        gameState = "Checkmate! Black wins!";
        running = false;
        score->changeBlackScore(1);
    } else if (check && !whitesTurn) {
        gameState = "Black is in check.";
    } else if (check && whitesTurn) {
        gameState = "White is in check.";
    } else if (noLegal) {
        gameState = "Stalemate!";
        running = false;
        score->changeWhiteScore(0.5);
        score->changeBlackScore(0.5);
    }
    return true;//the move is succesfull
}

bool Game::aiMove()
{
    if (whitesTurn && !whitePlayer->isAi()) {return false;}// need a function in player that can tell if its ai bool
    if (!whitesTurn && !blackPlayer->isAi()) {return false;}
    Player *curPlayer;
    if (whitesTurn) {curPlayer = whitePlayer;}
    if (!whitesTurn) {curPlayer = blackPlayer;}
    string theAiMove = curPlayer->makeMove(board);//this funciton should take board and make move based on ai level and
    //should give string in format "from to (promotion/if applicable)" eg: e2 e4, e7 e8 Q
    string startPos, endPos, promotion;
    stringstream aiStream(theAiMove);
    aiStream >> startPos >> endPos;
    if (!(aiStream >> promotion))
    {
        promotion = " ";
    }
    bool success = move(startPos, endPos, promotion);
    return success;
}

string Game::resign()
{
    if (running == false)
    {
        return "";
    }
    running = false;
    if (whitesTurn)
    {
        score->changeBlackScore(1);
        return "Black wins!";
    }
    else
    {
        score->changeWhiteScore(1);
        return "White wins!";
    }
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

string Game::getState() const
{
    return gameState;
}

string Game::getTurn() const
{
    if (running == false)//just in case newGame fails
    {
        return "";
    }
    if (whitesTurn)
    {
        return "White's turn!";
    }
    else
    {
        return "Black's turn!";
    }
}

string Game::printBoard() const 
{ 
    stringstream g;
    g << *board;
    return g.str();
}

string Game::printFinalScore() const 
{ 
    stringstream s;
    s << "Final Score:\n" << *score;
    return s.str();
}