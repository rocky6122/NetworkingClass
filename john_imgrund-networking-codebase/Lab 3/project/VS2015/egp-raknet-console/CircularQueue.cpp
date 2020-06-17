//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 


// Heavily Modified from https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
// We turned it into a template class in order to better suite our needs
// Delighter :D

#include "CircularQueue.h"

CircularQueue::CircularQueue(int size)
{
	mFront = mRear = -1;
	mSize = size;
	mQueue = new Event*[mSize];
}


CircularQueue::~CircularQueue()
{
	// empty for now
}

void CircularQueue::enqueue(Event* value)
{
	// Check if the array is full
	if ((mFront == 0 && mRear == mSize - 1) || (mRear == (mFront - 1) % (mSize - 1)))
	{
		//Error queue is full
		return;
	}
	// If the array is empty
	else if (mFront == -1)
	{
		// Insert first element
		mFront = mRear = 0;
		mQueue[mRear] = value;
	}
	else if (mRear == mSize - 1 && mFront != 0)
	{
		mRear = 0;
		mQueue[mRear] = value;
	}
	else
	{
		mRear++;
		mQueue[mRear] = value;
	}
}


Event* CircularQueue::dequeue()
{
	// Check if queue is empty
	if (mFront == -1)
	{
		// returns empty variable
		return nullptr;
	}

	Event* returnVal = mQueue[mFront];
	mQueue[mFront] = nullptr;	// sets the popped item to empty

	if (mFront == mRear)
	{
		mFront = -1;
		mRear = -1;
	}
	else if (mFront == mSize - 1)
	{
		mFront = 0;
	}
	else
	{
		mFront++;
	}
	
	return returnVal;
}

bool CircularQueue::isEmpty()
{
	return mFront == -1;
}


