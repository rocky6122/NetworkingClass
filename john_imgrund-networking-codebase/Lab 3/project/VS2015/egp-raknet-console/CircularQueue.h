//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once
#include "Event.h"

// Modified from https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/

class CircularQueue
{
public:
	CircularQueue(int size);
	~CircularQueue();

	void enqueue(Event* evnt);
	Event* dequeue();
	bool isEmpty();

private:
	int mFront;
	int mRear;
	int mSize;

	Event** mQueue;
};
