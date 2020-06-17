//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once
#include "networking.h"
#include "..\project\VS2015\egp-raknet-console\EventHeader.h"

///Game Functions

//Creates the gameField for the players
void initGameField(GameInformation* gi);

//Can only be called by the host
void spawnPlayers(GameInformation* gi);

//Displays the game field
bool displayGameField(GameInformation* gi, GameState *gs);

// Checks player against board conditions
void checkBoardState(GameInformation* gi, GameState* gs);

// Resets game board at correct intervals
bool cleanBoardState(GameInformation* gi);




///General Game Loop Functions

//Handles input from players
void handleInput(GameState* gs, GameInformation* gi);

//Handles gameplay Updates
void handleUpdate(GameState* gs, GameInformation* gi);

//Displays info to the screen
void handleDisplay(GameState* gs, GameInformation* gi);

