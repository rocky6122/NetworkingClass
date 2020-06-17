//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "game.h"

void initGameField(GameInformation* gi)
{
	for (int i = 0; i < gi->ROWS; i++)
	{
		for (int j = 0; j < gi->COLS; j++)
		{
			if (i == 0 || i == gi->ROWS - 1)
			{
				gi->gameField[i][j] = '#';
			}
			else if (j == 0 || j == gi->COLS - 1)
			{
				gi->gameField[i][j] = '#';
			}
			else
			{
				gi->gameField[i][j] = ' ';
			}
		}
	}
}

void spawnPlayers(GameInformation * gi)
{
	gi->gameField[9][1] = 'v';
	gi->myLoc.rows = gi->ROWS - 2;
	gi->myLoc.cols = 1;

	gi->gameField[1][19] = 't';
	gi->enemyLoc.rows = 1;
	gi->enemyLoc.cols = gi->COLS - 2;
}

bool displayGameField(GameInformation * gi, GameState* gs)
{
	bool shouldRender = false;

	if (gi->playerTick > -1)
	{
		shouldRender = true;

		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = (char)(65 + gi->playerTick--);

		if (gi->playerTick == -1)
		{
			//queue respawn event
			gs->mEventManager.getLocalQueue()->enqueue(new SpawnEvent(false));
			gs->mEventManager.getNetworkQueue()->enqueue(new SpawnEvent(true));
		}
	}
	else if (gi->enemyTick > -1)
	{
		shouldRender = true;

		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = (char)(65 + gi->enemyTick--);
	}

	for (int i = 0; i < gi->ROWS; i++)
	{
		for (int j = 0; j < gi->COLS; j++)
		{
			printf("%c",gi->gameField[i][j]);
		}

		//starts new line
		printf("\n");
	}

	return shouldRender;
}

void checkBoardState(GameInformation * gi, GameState* gs)
{
	// Check if player is shot
	if (gi->gameField[gi->myLoc.rows][gi->myLoc.cols] == ShootEvent::M_SHOOT_SYMBOL)
	{
		// HIT:
		// Step 1: don't receive more input
		//gi->amAlive = false;
		// Step 2: enqueue death event
		gs->mEventManager.getLocalQueue()->enqueue(new DieEvent(false));
		gs->mEventManager.getNetworkQueue()->enqueue(new DieEvent(true));
	}
}

bool cleanBoardState(GameInformation * gi)
{
	bool updateRender = false;

	// remove bullets from the screen
	for (int i = 0; i < gi->COLS - 1; i++)
	{
		for (int j = 0; j < gi->ROWS - 1; j++)
		{
			if (gi->gameField[j][i] == ShootEvent::M_SHOOT_SYMBOL)
			{
				gi->gameField[j][i] = ' ';

				if (!updateRender)
				{
					updateRender = true;
				}
			}
		}
	}

	return updateRender;
}

void handleInput(GameState* gs, GameInformation* gi)
{
	if(gs->canPlay && gi->amAlive)
	{
		//Handle Inputs here
		memcpy(gs->keyBoardState_0, gs->keyBoardState, sizeof(gs->keyBoardState));

		for (int i = 0; i < 256; i++)
		{
			gs->keyBoardState[i] = GetAsyncKeyState(i);
		}

		//Having all the movement checks together only allows for one movement event to be sent per frame
		if (gs->keyBoardState[37] && !gs->keyBoardState_0[37]) //Left Arrow Key
		{
			gs->mEventManager.getLocalQueue()->enqueue(new MoveEvent(MoveEvent::LEFT, false));
			gs->mEventManager.getNetworkQueue()->enqueue(new MoveEvent(MoveEvent::LEFT, true));
		}
		else if (gs->keyBoardState[38] && !gs->keyBoardState_0[38]) //Up Arrow Key
		{
			gs->mEventManager.getLocalQueue()->enqueue(new MoveEvent(MoveEvent::UP, false));
			gs->mEventManager.getNetworkQueue()->enqueue(new MoveEvent(MoveEvent::UP, true));
		}
		else if (gs->keyBoardState[39] && !gs->keyBoardState_0[39]) //Right Arrow Key
		{
			gs->mEventManager.getLocalQueue()->enqueue(new MoveEvent(MoveEvent::RIGHT, false));
			gs->mEventManager.getNetworkQueue()->enqueue(new MoveEvent(MoveEvent::RIGHT, true));
		}
		else if (gs->keyBoardState[40] && !gs->keyBoardState_0[40]) //Down Arrow Key
		{
			gs->mEventManager.getLocalQueue()->enqueue(new MoveEvent(MoveEvent::DOWN, false));
			gs->mEventManager.getNetworkQueue()->enqueue(new MoveEvent(MoveEvent::DOWN, true));
		}

		if (gs->keyBoardState[32] && !gs->keyBoardState_0[32]) //Space Bar
		{
			gs->mEventManager.getLocalQueue()->enqueue(new ShootEvent(gi->lastDirection, false));
			gs->mEventManager.getNetworkQueue()->enqueue(new ShootEvent(gi->lastDirection, true));
		}

		if (gs->keyBoardState[27] && !gs->keyBoardState_0[27]) // Escape Key
		{
			gs->isPlaying = 0;
			gs->canPlay = 0;
		}
	}
}


void handleUpdate(GameState * gs, GameInformation* gi)
{
	//gs->canPlay = 1;

	// If game hasn't started 
	// And two players are in, 
	// start the game
	if (gs->startGame)
	{
		initGameField(gi);
		spawnPlayers(gi);

		gs->startGame = 0;		// Ensures we do not run this if block again
		gs->canPlay = 1;		// flag for player input
		gs->updateRender = 1;	// Automatically show the board
	}

	// Check for board conditions
	checkBoardState(gi, gs);

	// clean board state
	cleanBoardState(gi);

	// Handle all events in queue
	if (gs->mEventManager.handleEvents(gi))
	{
		gs->updateRender = 1;
	}
}

void handleDisplay(GameState *gs, GameInformation* gi)
{ 

	if (gs->updateRender)
	{
		system("cls");

		//Displays the gameField
		if (!displayGameField(gi, gs))
		{
			gs->updateRender = 0;
		}
	}
}


