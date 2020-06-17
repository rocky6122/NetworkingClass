//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/13/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <conio.h> //For User input

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

enum GameMessages
{
	ID_CUSTOM_MESSAGE_START = ID_USER_PACKET_ENUM,

	ID_CHAT_MESSAGE_1,
	ID_SERVER_MESSAGE_1,
	SEND_USER_DATA,
	SEND_MESSAGE_REQUEST,
	SEND_MESSAGE_DELIVERY
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

#pragma pack (pop)

//Returns whether or not the user wants to join or host a server via an int.
int lobbyState();

//Removes newline from the end of fgets for cleaner work using C.
// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
char* fgetstr(char*, int, FILE*);

//Returns the GUID of the targetUser
RakNet::RakNetGUID getGUID(UserData * array, char targetName[15], int arraySize);