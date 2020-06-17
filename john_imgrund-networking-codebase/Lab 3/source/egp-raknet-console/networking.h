//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

#include "..\project\VS2015\egp-raknet-console\EventManager.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

enum GameMessages
{
	ID_CUSTOM_MESSAGE_START = ID_USER_PACKET_ENUM,

	ID_CHAT_MESSAGE_1,
	ID_SERVER_MESSAGE_1,
	SEND_USER_DATA,
	GAME_EVENT
};

#pragma pack(push, 1)

struct UserData
{
	unsigned char typeID = SEND_USER_DATA;

	char userName[31];
	RakNet::SystemAddress address;
	RakNet::RakNetGUID userGUID;
	int timeStamp;
};

struct ServerMessage
{
	unsigned char typeID;

	RakNet::RakNetGUID hostID;
	char userName[31];
	char message[64];
};

struct GameEvent
{
	unsigned char typeID = GAME_EVENT;

	EventType type;

	//Variables for Move Event
	MoveEvent::direction direct;

	//Variables for Shoot Event
	GameInformation::Directions lastDirect;

	//Variables for Death Event


};



#pragma pack (pop)



struct GameState
{
	// generic game state
	RakNet::RakPeerInterface *peer;
	int isServer = 0;
	unsigned short serverPort;
	unsigned short keyBoardState[256], keyBoardState_0[256];
	RakNet::RakNetGUID host_id;


	//Personal Variables
	char userName[512];
	UserData* clientData;
	int currentClientNum = 0;
	const unsigned int MAXCLIENTS = 2;
	EventManager mEventManager;
	


	//FLAGS

	//Designates the game is ready to start
	char startGame = 0;
	// Designates the characters can move
	char canPlay = 0;
	// Designates ready to update console
	char updateRender = 0;
	// Designates to leave networking loop
	char isLeaving = 0;
	// Designates to exit full program
	char isRunning = 1;
	// Designates to run the game loop
	char isPlaying = 0;

	// Designates ready to send packet
	char send = 0;
};

void getUserName(GameState* gs);

//Proccesses all the networking
void handleNetworking(GameState* gs);

//Returns whether or not the user wants to join or host a server via an int.
int lobbyState();

void joinGame(GameState* gs);

void hostGame(GameState* gs);

void leaveGame(GameState* gs);

void sendEvents(GameState* gs);

//Returns the GUID of the targetUser
RakNet::RakNetGUID getGUID(UserData * array, char targetName[15], int arraySize);

// Removes newline from the end of fgets for cleaner work using C.
// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
char* fgetstr(char*, int, FILE*);
