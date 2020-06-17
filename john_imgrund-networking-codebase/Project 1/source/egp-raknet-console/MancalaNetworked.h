//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once
#include "mancala.h"
#include "mancalaFunctions.h"

//Set up the gameData
void setUpGame(GameState* gs);

//Turn for the newtorked version
void handleTurn(GameState* gs);

// Modified version of print board to be used online
void MancalaNetworkPrintBoard(int gameBoard[], char topPlayer[15], char bottomPlayer[15], int topIndex, int bottomIndex);