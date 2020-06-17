//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "EventManager.h"

EventManager::EventManager()
{
	//Creating the local queues
	mLocalQueue = new CircularQueue(16);
	mNetworkQueue = new CircularQueue(16);
}

EventManager::~EventManager()
{
	delete mLocalQueue;
	delete mNetworkQueue;
}

bool EventManager::handleEvents(GameInformation* gi)
{
	bool handled = false;

	while (!mLocalQueue->isEmpty())
	{
		if (!handled)
		{
			handled = true;
		}

		Event* theEvent = mLocalQueue->dequeue();

		if (theEvent->getType() == MOVE_EVENT)
		{
			MoveEvent* moveEvent = static_cast<MoveEvent*>(theEvent);
			moveEvent->execute(gi);
		}
		else if (theEvent->getType() == SHOOT_EVENT)
		{
			ShootEvent* shootEvent = static_cast<ShootEvent*>(theEvent);
			shootEvent->execute(gi);
		}
		else if (theEvent->getType() == DIE_EVENT)
		{
			DieEvent* dieEvent = static_cast<DieEvent*>(theEvent);
			dieEvent->execute(gi);
		}
		else if (theEvent->getType() == SPAWN_EVENT)
		{
			SpawnEvent* spawnEvent = static_cast<SpawnEvent*>(theEvent);
			spawnEvent->execute(gi);
		}

		delete theEvent; // deleting the pointer
	}

	return handled;
}
