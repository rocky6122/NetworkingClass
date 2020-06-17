//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once

#include "MancalaNetworked.h"


void handleInput(GameState* gs);

//Checks the message for any commands
void parseMessage(GameState* gs);

//Preps the private message for correct sending method
void preparePrivateMessage(GameState *gs);


void handleUpdate(GameState* gs);

//Displays info to the screen
void handleDisplay(GameState* gs);

