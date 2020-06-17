//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once

// Enum used as tags for each event
// which lets us know which event to cast to
enum EventType
{
	INVALID_EVENT = -1,
	MOVE_EVENT,
	SHOOT_EVENT,
	DIE_EVENT,
	SPAWN_EVENT
};

class Event
{
public:
	Event();
	Event(EventType t);
	virtual ~Event();

	EventType getType() const { return mType; }
	virtual void execute() = 0;

protected:
	EventType mType;
};

