//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "networking.h"

void getUserName(GameState * gs)
{
	printf("Please enter a username for this chat instance (15 characters or less):\n");
	fgetstr(gs->userName, 512, stdin);
}

void handleNetworking(GameState * gs)
{
	//READ
	RakNet::Packet *packet;

	for (
		packet = gs->peer->Receive();
		packet;
		gs->peer->DeallocatePacket(packet), packet = gs->peer->Receive()
		)
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			break;
		case ID_REMOTE_CONNECTION_LOST:
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			//Gathers user data to send to the server as reference.
			UserData data[1];
			strcpy(data->userName, gs->userName);
			data->userGUID = gs->peer->GetMyGUID();
			data->address = gs->peer->GetSystemAddressFromGuid(gs->peer->GetMyGUID());
			data->timeStamp = RakNet::GetTime();
			gs->peer->Send((char*)(data), sizeof(UserData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
		break;
		case ID_NEW_INCOMING_CONNECTION:
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (gs->isServer) {
				printf("A client has disconnected.\n");
			}
			else {
				printf("We have been disconnected.\n");
			}
			break;
		case ID_CONNECTION_LOST:
			if (gs->isServer) {
				printf("A client lost the connection.\n");
			}
			else {
				printf("Connection lost.\n");
			}
			break;

		case ID_CHAT_MESSAGE_1:
			printf("DEBUG MESSAGE: received packet ID_GAME_MESSAGE_1.\n");
			{
			}
			break;

		case ID_SERVER_MESSAGE_1:
		{
			//Lets users know someone has joined
			const ServerMessage *MSG = (ServerMessage *)packet->data;

			gs->host_id = MSG->hostID;

			printf(MSG->userName);
			printf(MSG->message);
		}
		break;

		case SEND_MESSAGE_REQUEST:
		{
			SendMessageRequest *MSG = (SendMessageRequest *)packet->data;

			if (MSG->targetName[0] != '\0') //Private message
			{
				MessageDelivery delivery[1];
				strcpy(delivery->userName, MSG->userName);
				strcpy(delivery->message, MSG->message);
				delivery->isPrivate = true;


				if (delivery->isPrivate)
				{
					printf("[%s (Whispered)]: ", MSG->userName);
					printf("%s ", MSG->message);
					printf("[To %s]", MSG->targetName);
				}
				else
				{
					printf("[%s]: ", MSG->userName);
					printf("%s\n", MSG->message);
				}


				gs->peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, getGUID(gs->clientData, MSG->targetName, gs->maxClients), false);

				addMessageToLog(gs, MessageLog(delivery->userName, delivery->message, delivery->isPrivate));
			}
			else //Public Message
			{
				MessageDelivery delivery[1];
				strcpy(delivery->userName, MSG->userName);
				strcpy(delivery->message, MSG->message);

				printf("[%s]: ", MSG->userName);
				printf("%s\n", MSG->message);

				gs->peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, MSG->userGUID, true);

				addMessageToLog(gs, MessageLog(delivery->userName, delivery->message, delivery->isPrivate));
			}
		}
		break;

		case SEND_MESSAGE_DELIVERY:
		{
			MessageDelivery *MSG = (MessageDelivery *)packet->data;

			if (MSG->isPrivate) //Private Message
			{
				printf("[%s (Whispered)]: ", MSG->userName);
				printf("%s\n", MSG->message);
			}
			else //Public Message
			{
				printf("[%s]: ", MSG->userName);
				printf("%s\n", MSG->message);
			}



			// Record message to log
			addMessageToLog(gs, MessageLog(MSG->userName, MSG->message, MSG->isPrivate));


			// Set flag to render new input
			gs->updateRender = 1;
		}
		break;

		case SEND_USER_DATA:
		{

			if (gs->currentClientNum == 0) //Host (Sets host data into the array on first run through)
			{
				UserData hostData;
				strcpy(hostData.userName, gs->userName);
				hostData.userGUID = gs->peer->GetMyGUID();
				hostData.address = gs->peer->GetSystemAddressFromGuid(gs->peer->GetMyGUID());
				hostData.timeStamp = RakNet::GetTime();
				gs->clientData[gs->currentClientNum] = hostData;

				gs->currentClientNum++;
			}

			UserData *userData = (UserData *)packet->data;

			gs->clientData[gs->currentClientNum] = *userData;

			// Selects the first person that joins the lobby to be a player
			if (gs->currentClientNum == 1)
			{
				PlayerSelection selection[1];
				gs->peer->Send((char*)(selection), sizeof(PlayerSelection), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				
			gs->currentClientNum++;

			printf("%s has joined the chatroom.\n", userData->userName);

			ServerMessage message[1];
			message->typeID = ID_SERVER_MESSAGE_1;
			message->hostID = gs->peer->GetMyGUID();
			strcpy(message->userName, userData->userName);
			strcpy(message->message, " has joined the chat room.\n");

			gs->peer->Send((char*)(message), sizeof(ServerMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

			
		}
		break;

		case ID_PLAYER_SELECT:
		{
			gs->isPlayer = 1;
		}
		break;

		case ID_GAME_UPDATE:
		{
			if (gs->isPlayer)
			{
				gs->myTurn = 1;
			}

			gs->updateRender = 1;

			GameData *data = (GameData *)packet->data;

			//Sets all data into the GameData packet to send
			memcpy(gs->mData.player0Name, data->player0Name, sizeof(data->player0Name));
			memcpy(gs->mData.player1Name, data->player1Name, sizeof(data->player1Name));
			memcpy(gs->mData.scores,  data->scores, sizeof(data->scores));
			memcpy(gs->mData.gameBoard, data->gameBoard, sizeof(data->gameBoard));
			gs->mData.winner = data->winner;

			if (gs->isServer)
			{
				gs->peer->Send((char*)(data), sizeof(GameData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
			}

		}
		break;
		
		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
	}

	//WRITES
	if (gs->send)
	{
		//CLIENT WRITE
		if (!gs->isServer)
		{
			SendMessageRequest msgRequest[1];

			msgRequest->userGUID = gs->peer->GetMyGUID();

			strcpy(msgRequest->userName, gs->userName);
			strcpy(msgRequest->targetName, gs->msgData.targetName);
			strcpy(msgRequest->message, gs->msgData.message);

			gs->peer->Send((char*)(msgRequest), sizeof(SendMessageRequest), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->host_id, false);

			addMessageToLog(gs, MessageLog(msgRequest->userName, msgRequest->message, !msgRequest->targetName[0] == '\0'));
		}
		else //SERVER WRITE
		{
			MessageDelivery delivery[1];
			strcpy(delivery->userName, gs->userName);
			strcpy(delivery->message, gs->msgData.message);

			if (gs->msgData.targetName[0] != '\0') //Checks if it is private
			{
				delivery->isPrivate = true;
				gs->peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, getGUID(gs->clientData, gs->msgData.targetName, gs->maxClients), false);
			}
			else //Public Message
			{
				gs->peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->peer->GetMyGUID(), true);
			}

			addMessageToLog(gs, MessageLog(delivery->userName, delivery->message, delivery->isPrivate));
		}


		gs->send = 0;
	}

	if (gs->sendGameData)
	{
		GameData data[1];
		
		//Sets all data into the GameData packet to send
		memcpy(data->player0Name, gs->mData.player0Name, sizeof(gs->mData.player0Name));
		memcpy(data->player1Name, gs->mData.player1Name, sizeof(gs->mData.player1Name));
		memcpy(data->scores, gs->mData.scores, sizeof(gs->mData.scores));
		memcpy(data->gameBoard, gs->mData.gameBoard, sizeof(gs->mData.gameBoard));
		data->winner = gs->mData.winner;


		if (gs->isServer)
		{
			gs->peer->Send((char*)(data), sizeof(GameData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		else
		{
			gs->peer->Send((char*)(data), sizeof(GameData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->host_id, false);
		}
		gs->sendGameData = 0;
	}
}

int lobbyState()
{
	char str[512];
	int temp;

	do
	{
		printf("(J)oin a lobby, (H)ost a lobby, or (Q)uit?\n(Note: Host is always a player of Mancala. First person to join is also a player.)\n");
		fgetstr(str, 512, stdin);

		if ((str[0] == 'j') || (str[0] == 'J'))
		{
			temp = 0; //Joins a chatroom
		}
		else if ((str[0] == 'h') || (str[0] == 'H'))
		{
			temp = 1; //Hosts a chatroom
		}
		else if ((str[0] == 'q') || (str[0] == 'Q'))
		{
			temp = 2; //Quits the program
		}
	} while (temp != 0 && temp != 1 && temp != 2);

	return temp;
}

void joinChatroom(GameState* gs)
{
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];
	char str2[bufferSz];

	//Finds the chatroom to join
	RakNet::SocketDescriptor sd;
	gs->peer->Startup(1, &sd, 1);
	gs->peer->Startup(1, &sd, 1);

	printf("Enter server IP or hit enter for 127.0.0.1\n");
	fgets(str, bufferSz, stdin);

	if (str[0] == '\n')
	{
		strcpy(str, "127.0.0.1");
	}

	printf("Enter Server Port Number: \n");
	fgets(str2, bufferSz, stdin);
	gs->serverPort = atoi(str2);

	printf("Joining the chatroom.\n");
	gs->peer->Connect(str, gs->serverPort, 0, 0);
	gs->isChatting = true;

	printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n /help to display these commands.\n");
}

void hostChatroom(GameState * gs)
{
	gs->isServer = 1;
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];
	char str2[bufferSz];

	printf("Enter maximum number of people who can join the chatroom: \n");
	fgets(str, bufferSz, stdin);

	gs->maxClients = atoi(str);

	printf("Enter Server Port Number: \n");
	fgets(str2, bufferSz, stdin);

	gs->serverPort = atoi(str2);
	RakNet::SocketDescriptor sd(gs->serverPort, 0);
	gs->peer->Startup(gs->maxClients, &sd, 1);

	gs->clientData = new UserData[gs->maxClients];

	// We need to let the server accept incoming connections from the clients
	printf("Starting the Chatroom.\n");
	gs->peer->SetMaximumIncomingConnections(gs->maxClients);
	gs->isChatting = true;
	gs->isPlayer = 1;

	printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n Press ` to get client list.\n /help to display these commands.\n");

}

void leaveChatroom(GameState * gs)
{
	SendMessageRequest msgRequest[1];
	msgRequest->userGUID = gs->peer->GetMyGUID();
	strcpy(msgRequest->userName, gs->userName);
	strcpy(msgRequest->message, "has left the chatroom.\n");
	msgRequest->targetName[0] = '\0';

	gs->peer->Send((char*)(msgRequest), sizeof(SendMessageRequest), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->host_id, false);

	gs->isLeaving = 0;
	gs->isServer = 0;
	gs->isChatting = false;
}

RakNet::RakNetGUID getGUID(UserData * array, char targetName[15], int arraySize)
{
	RakNet::RakNetGUID temp;

	char name[15];
	char testName[15];

	strcpy(name, targetName);


	for (int i = 0; i < arraySize; i++)
	{
		strcpy(testName, array[i].userName);

		if (strcmp(testName, name) == 0)
		{
			temp = array[i].userGUID;
			break;
		}
	}

	return temp;
}

// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
// Removes new line character at the end of line if it exists
char* fgetstr(char *string, int n, FILE *stream)
{
	char *result;

	result = fgets(string, n, stream);

	if (!result)
	{
		return(result);
	}

	if (string[strlen(string) - 1] == '\n')
	{
		string[strlen(string) - 1] = 0;
	}

	return(string);
}

void addMessageToLog(GameState * gs, MessageLog msg)
{
	if (gs->log.size() >= 10)
	{
		gs->log.erase(gs->log.begin());
	}

	gs->log.push_back(msg);
}