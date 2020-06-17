//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once
#include "GameInformation.h"
#include "Event.h"

class MoveEvent : public Event
{
public:
	enum direction
	{
		LEFT = 0,
		UP,
		RIGHT,
		DOWN
	};

	MoveEvent(direction direct, bool isNetworked);
	~MoveEvent();

	void execute();
	void execute(GameInformation* gi);

	direction getDirection() const { return mNewDirect; }
	bool getNetworked() const { return mIsNetworked; }

private:
	direction mNewDirect;
	bool mIsNetworked;
};

