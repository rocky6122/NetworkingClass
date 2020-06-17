//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once
#include "networking.h"

// take a turn
// returns resulting player and cup indices as pointers
// returns pointer to cup that can be stolen from as pointer (null if not)
// return value is 1 if the player goes again (landed in their mancala)
int MancalaTurn(int *const *const count, int playerIndex, int cupIndex,
	int *const playerIndex_result, int *const cupIndex_result, int **const steal_ptr);

// Mancala game: straight-forward, two human players
// returns index of winner, -1 if tie
// saves final scores in optional pointer passed as argument (scores_out)
int MancalaSimple(const char *const *const playerName, int *const scores_out);

// Manages a game loop for local Mancala
void playLocalMancala(GameState* gs);

