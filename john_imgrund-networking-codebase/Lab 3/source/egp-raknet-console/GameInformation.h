//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once

struct PlayerLocation
{
	int rows;
	int cols;
};

struct GameInformation
{
	// Directions used for storing last movement
	enum Directions
	{
		LEFT = 0,
		UP,
		RIGHT,
		DOWN
	};

	///Game Variables

	//structure: gameField[ROWS][COLS] or (y,x)
	char gameField[11][21];
	PlayerLocation myLoc;
	PlayerLocation enemyLoc;
	Directions lastDirection = UP;
	
	const int ROWS = 11;
	const int COLS = 21;

	//Flags

	bool amAlive = true;
	int enemyTick = -1;
	int playerTick = -1;
};