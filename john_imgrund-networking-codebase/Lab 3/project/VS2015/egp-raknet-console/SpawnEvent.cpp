//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 


#include "SpawnEvent.h"

SpawnEvent::SpawnEvent(bool isNetworked) : Event(SPAWN_EVENT)
{
	mIsNetworked = isNetworked;

	mPlayerSpawnLoc.rows = 9;
	mPlayerSpawnLoc.cols = 1;
	
	mEnemySpawnLoc.rows = 1;
	mEnemySpawnLoc.cols = 19;
}


SpawnEvent::~SpawnEvent()
{
}

void SpawnEvent::execute()
{
}

void SpawnEvent::execute(GameInformation * gi)
{
	if (mIsNetworked)
	{
		// Resets where the enemy was
		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = ' ';

		// Sets and displays new location
		gi->enemyLoc = mEnemySpawnLoc;
		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = 't';
	}
	else
	{
		// Reset
		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = ' ';

		// Set and prepare display
		gi->myLoc = mPlayerSpawnLoc;
		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = 'v';

		gi->amAlive = true;
	}
}
