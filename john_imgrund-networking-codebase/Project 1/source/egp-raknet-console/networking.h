//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
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

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

enum GameMessages
{
	ID_CUSTOM_MESSAGE_START = ID_USER_PACKET_ENUM,

	ID_CHAT_MESSAGE_1,
	ID_SERVER_MESSAGE_1,
	SEND_USER_DATA,
	SEND_MESSAGE_REQUEST,
	SEND_MESSAGE_DELIVERY,
	ID_PLAYER_SELECT,
	ID_GAME_UPDATE,
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

struct SendMessageRequest
{
	unsigned char typeID = SEND_MESSAGE_REQUEST;

	char targetName[31];
	// if this is empty, send to everyone;

	RakNet::RakNetGUID userGUID;
	char userName[31];
	char message[512];
};

struct MessageDelivery
{
	unsigned char typeID = SEND_MESSAGE_DELIVERY;

	bool isPrivate = false;
	char userName[31];
	char message[512];
};

struct PlayerSelection
{
	unsigned char typeID = ID_PLAYER_SELECT;
};

struct GameData
{
	unsigned char typeID = ID_GAME_UPDATE;

	//Variables
	char player0Name[15];
	char player1Name[15];
	int scores[2];

	int gameBoard[16];

	int winner = -1;

};

#pragma pack (pop)

struct MessageLog
{
	char username[15]; // person who sent it
	char message[512];
	bool isPrivate;

	MessageLog(char name[], char msg[], bool priv)
	{
		strcpy(username, name);
		strcpy(message, msg);
		isPrivate = priv;
	}
};

struct GameState
{
	// generic game state
	RakNet::RakPeerInterface *peer;
	int isServer = 0;
	unsigned short serverPort;

	unsigned short keyBoardState[256], keyBoardState_0[256];

	//Personal Variables
	char userName[512];
	UserData* clientData;
	int currentClientNum = 0;
	unsigned int maxClients;
	std::vector<MessageLog> log;

	RakNet::RakNetGUID host_id;

	GameData mData;

	SendMessageRequest msgData;
	unsigned char msgLength;

	//FLAGS

	// Keeps player in chat lobby
	bool isChatting = false;
	// Designates whether client is playing mancala
	// or chatting
	char isPlayer = 0;
	char updateRender = 0;
	// Designates to leave networking loop
	char isLeaving = 0;
	// Designates to exit full program
	char isRunning = 1;
	char isTyping = 0;
	char send;
	char sendGameData = 0;

	//Game Flags
	char gameStarted = 0;
	char myTurn = 0;
};

void getUserName(GameState* gs);

//Proccesses all the networking
void handleNetworking(GameState* gs);

//Returns whether or not the user wants to join or host a server via an int.
int lobbyState();

void joinChatroom(GameState* gs);

void hostChatroom(GameState* gs);

void leaveChatroom(GameState* gs);

// Adds message to local message log
void addMessageToLog(GameState* gs, MessageLog msg);

//Returns the GUID of the targetUser
RakNet::RakNetGUID getGUID(UserData * array, char targetName[15], int arraySize);

// Removes newline from the end of fgets for cleaner work using C.
// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
char* fgetstr(char*, int, FILE*);
