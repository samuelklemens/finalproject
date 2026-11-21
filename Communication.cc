#include "Communication.h"
#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

Communication::Communication(Game* game) : game{game} {}

Communication::~Communication() {}

void Communication::communicate()
{   
    string command;
    string position;
    char piece;
    while (cin >> command)
    {
        //find what command it is and do the appropriate game command
        if (!game->isRunning() && command != "game" && command != "setup")
        {//cannot do any other command unless its setup first
            cout << "To start game give players!." << endl;
        }
        else
        {
            if (command == "game"){//setup players with game command
                string white, black;
                cin >> white;
                cin >> black;
                if (!game->newGame(white, black))
                {
                    cout << "Invalid game!" << endl;
                }
                else
                {
                    cout << game->getBoard(); 
                }
            } else if (command == "setup") {//setup command
                if (!game->isRunning())
                {
                    game->setup();
                    while (cin >> command)
                    {
                        if (command == "+") { //adding in setup
                            cin >> command;
                            if (command.length() != 1)
                            {
                                cout << "Invalid Piece Length!" << endl;
                            }
                            else
                            {
                                piece = command[0];
                                cin >> position;
                                if (!game->setupSquare(piece, position))
                                {
                                    cout << "Add was invalid!" << endl;
                                }
                                else
                                {
                                    cout << game->getBoard(); 
                                }
                            }//end of adding in setup
                        } else if (command == "-") { // removing in setup
                            cin >> position;
                            if (!game->setupRemoveSquare(position))
                            {
                                cout << "Remove was invalid!" << endl;
                            }
                            else
                            {
                                cout << game->getBoard();
                            }// end removing in setup
                        } else if (command == "=") {
                            cin >> command;

                        } else if (command == "done") {// done in setup
                                if (game->done()) {break;}
                                else
                                {
                                    cout << "The board is invalid!" << endl;
                                }//end done in setup
                        } else {
                            cout << "Invalid setup command!" << endl;
                        }
                    }
                }
                else
                {
                    cout << "Cant setup during a game!" << endl;
                }
            } else if (command == "move") {//move command
                string startPos, endPos;
                cin >> startPos;
                cin >> endPos;
                if (!game->move(startPos, endPos))
                {
                    cout << "Invalid move!" << endl;
                }
                else
                {
                    cout << game->getBoard(); 
                }
            } else if (command == "resign") {//resign command
                game->resign();
            } else{//invalid command
                cout << "Please enter valid command: 'game, setup, move, resign'!" << endl;
            }
        }
    }
}