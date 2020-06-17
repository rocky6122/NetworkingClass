//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 


#include "ShootEvent.h"

ShootEvent::ShootEvent(GameInformation::Directions dir, bool isNetworked) :Event(SHOOT_EVENT)
{
	mDir = dir;
	mIsNetworked = isNetworked;
}


ShootEvent::~ShootEvent()
{
}

void ShootEvent::execute()
{
}

// Shoot Event creates a "laser" of '*' characters 
// that spread out for the entire column/row that the 
// player was last moving in
void ShootEvent::execute(GameInformation* gi)
{
	if (mIsNetworked) // Other player caused event
	{
		switch (mDir)
		{
		case GameInformation::LEFT:
		{
			for (int i = gi->enemyLoc.cols + 1; i < gi->COLS - 1; i++)
			{
				gi->gameField[gi->enemyLoc.rows][i] = M_SHOOT_SYMBOL;
			}
		}
		break;

		case GameInformation::UP:
		{
			for (int i = gi->enemyLoc.rows + 1; i < gi->ROWS - 1; i++)
			{
				gi->gameField[i][gi->enemyLoc.cols] = M_SHOOT_SYMBOL;
			}
		}
		break;

		case GameInformation::RIGHT:
		{
			for (int i = gi->enemyLoc.cols - 1; i > 0; i--)
			{
				gi->gameField[gi->enemyLoc.rows][i] = M_SHOOT_SYMBOL;
			}
		}
		break;

		case GameInformation::DOWN:
		{
			for (int i = gi->enemyLoc.rows - 1; i > 0; i--)
			{
				gi->gameField[i][gi->enemyLoc.cols] = M_SHOOT_SYMBOL;
			}
		}
		break;
		}
	}
	else // Local player caused event
	{
		switch (mDir)
		{
			case GameInformation::LEFT:
			{
				for (int i = gi->myLoc.cols - 1; i > 0; i--)
				{
					gi->gameField[gi->myLoc.rows][i] = M_SHOOT_SYMBOL;
				}
			}
			break;

			case GameInformation::UP:
			{
				for (int i = gi->myLoc.rows - 1; i > 0; i--)
				{
					gi->gameField[i][gi->myLoc.cols] = M_SHOOT_SYMBOL;
				}
			}
			break;

			case GameInformation::RIGHT:
			{
				for (int i = gi->myLoc.cols + 1; i < gi->COLS - 1; i++)
				{
					gi->gameField[gi->myLoc.rows][i] = M_SHOOT_SYMBOL;
				}
			}
			break;

			case GameInformation::DOWN:
			{
				for (int i = gi->myLoc.rows + 1; i < gi->ROWS - 1; i++)
				{
					gi->gameField[i][gi->myLoc.cols] = M_SHOOT_SYMBOL;
				}
			}
			break;
		}
	}
}
