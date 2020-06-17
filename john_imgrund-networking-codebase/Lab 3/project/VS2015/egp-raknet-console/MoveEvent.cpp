//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 


#include "MoveEvent.h"

MoveEvent::MoveEvent(direction direct, bool isNetworked) : Event(MOVE_EVENT)
{
	mNewDirect = direct;
	mIsNetworked = isNetworked;
}


MoveEvent::~MoveEvent()
{
}

void MoveEvent::execute()
{
}

void MoveEvent::execute(GameInformation* gi)
{
	//Move the player in the correct direction
	if (mIsNetworked) //Moving Enemy
	{
		//rows == vertical
		//cols == horizontal

		//Delete your enemy from its current position
		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = ' ';

		//Might have to move the enemy opposite of what the actual direction says due to the boards being flipped if ur on another PC
		switch (mNewDirect)
		{
		case LEFT:
			if (gi->enemyLoc.cols < gi->COLS - 2)
			{
				gi->enemyLoc.cols += 1;
			}
			break;
		case UP:
			if (gi->enemyLoc.rows < gi->ROWS - 2)
			{
				gi->enemyLoc.rows += 1;
			}
			break;
		case RIGHT:
			if (gi->enemyLoc.cols > 1)
			{
				gi->enemyLoc.cols -= 1;
			}
			break;
		case DOWN:
			if (gi->enemyLoc.rows > 1)
			{
				gi->enemyLoc.rows -= 1;
			}
			break;
		}

		//Add enemy in at your new position
		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = 't';
		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = 'v';
	}
	else //Moving Self
	{
		//rows == vertical
		//cols == horizontal

		//Delete your own player from its current position
		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = ' ';

		switch (mNewDirect)
		{
		case LEFT:
			if (gi->myLoc.cols > 1)
			{
				gi->myLoc.cols -= 1;

				gi->lastDirection = GameInformation::LEFT;
			}
			break;
		case UP:
			if (gi->myLoc.rows > 1)
			{
				gi->myLoc.rows -= 1;
				gi->lastDirection = GameInformation::UP;
			}
			break;
		case RIGHT:
			if (gi->myLoc.cols < gi->COLS - 2)
			{
				gi->myLoc.cols += 1;
				gi->lastDirection = GameInformation::RIGHT;
			}
			break;
		case DOWN:
			if (gi->myLoc.rows < gi->ROWS - 2)
			{
				gi->myLoc.rows += 1;
				gi->lastDirection = GameInformation::DOWN;
			}
			break;
		}

		//Add player in at your new position
		gi->gameField[gi->enemyLoc.rows][gi->enemyLoc.cols] = 't';
		gi->gameField[gi->myLoc.rows][gi->myLoc.cols] = 'v';
	}
}
