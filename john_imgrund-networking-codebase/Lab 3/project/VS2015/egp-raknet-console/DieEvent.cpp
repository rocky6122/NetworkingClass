//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "DieEvent.h"


DieEvent::DieEvent(bool isNetworked) : Event(DIE_EVENT)
{
	mIsNetworked = isNetworked;
}


DieEvent::~DieEvent()
{
}

void DieEvent::execute()
{
}

// Die event sets flag for displaying a death animation
void DieEvent::execute(GameInformation * gi)
{
	if (mIsNetworked)
	{
		gi->enemyTick = 26;
	}
	else
	{
		gi->playerTick = 26;
		gi->amAlive = false;
	}
}
