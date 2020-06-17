//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 3
//10/4/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "networking.h"

void getUserName(GameState * gs)
{
	printf("Please enter a username for this instance (15 characters or less):\n");
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

				gs->startGame = 1;
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

				gs->currentClientNum++;

				printf("%s has joined the chatroom.\n", userData->userName);

				ServerMessage message[1];
				message->typeID = ID_SERVER_MESSAGE_1;
				message->hostID = gs->peer->GetMyGUID();
				strcpy(message->userName, userData->userName);
				strcpy(message->message, " has joined the chat room.\n");

				gs->peer->Send((char*)(message), sizeof(ServerMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

				gs->startGame = 1;
			}
			break;

			case GAME_EVENT:
			{
				const GameEvent *gameEvent = (GameEvent *)packet->data;

				switch (gameEvent->type)
				{
				case MOVE_EVENT:
				{
					gs->mEventManager.getLocalQueue()->enqueue(new MoveEvent(gameEvent->direct, true));
				}
					break;
				
				case SHOOT_EVENT:
				{
					gs->mEventManager.getLocalQueue()->enqueue(new ShootEvent(gameEvent->lastDirect, true));
				}
					break;
				
				case DIE_EVENT:
				{
					gs->mEventManager.getLocalQueue()->enqueue(new DieEvent(true));
				}
					break;
				
				case SPAWN_EVENT:
				{
					gs->mEventManager.getLocalQueue()->enqueue(new SpawnEvent(true));
				}
					break;	
				}

			}
			break;	
		}
	}

	//WRITES
	sendEvents(gs);
}

int lobbyState()
{
	char str[512];
	int temp;

	do
	{
		printf("(J)oin a lobby, (H)ost a lobby, or (Q)uit?\n");
		fgetstr(str, 512, stdin);

		if ((str[0] == 'j') || (str[0] == 'J'))
		{
			temp = 0; //Joins a game
		}
		else if ((str[0] == 'h') || (str[0] == 'H'))
		{
			temp = 1; //Hosts a game
		}
		else if ((str[0] == 'q') || (str[0] == 'Q'))
		{
			temp = 2; //Quits the program
		}
	} while (temp != 0 && temp != 1 && temp != 2);

	return temp;
}

void joinGame(GameState* gs)
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

	printf("Joining the game.\n");
	gs->peer->Connect(str, gs->serverPort, 0, 0);

	gs->isPlaying = 1;
}

void hostGame(GameState * gs)
{
	gs->isServer = 1;
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];
	char str2[bufferSz];

	printf("Enter Server Port Number: \n");
	fgets(str2, bufferSz, stdin);

	gs->serverPort = atoi(str2);
	RakNet::SocketDescriptor sd(gs->serverPort, 0);
	gs->peer->Startup(gs->MAXCLIENTS, &sd, 1);

	gs->clientData = new UserData[gs->MAXCLIENTS];

	// We need to let the server accept incoming connections from the clients

	system("cls");
	printf("Starting the Game.\n");
	gs->peer->SetMaximumIncomingConnections(gs->MAXCLIENTS);

	printf("Waiting for second player...");

	gs->isPlaying = 1;
}

void leaveGame(GameState * gs)
{
	// Make a better way to signify that we are leaving
}

void sendEvents(GameState * gs)
{
	while (!gs->mEventManager.getNetworkQueue()->isEmpty())
	{
		GameEvent gameEvent[1];

		Event* theEvent = gs->mEventManager.getNetworkQueue()->dequeue();

		if (theEvent->getType() == MOVE_EVENT)
		{
			MoveEvent* moveEvent = static_cast<MoveEvent*>(theEvent);
			
			//Needed variables for this type of event
			gameEvent->type = MOVE_EVENT;
			gameEvent->direct = moveEvent->getDirection();
		}
		else if (theEvent->getType() == SHOOT_EVENT)
		{
			ShootEvent* shootEvent = static_cast<ShootEvent*>(theEvent);
			//Needed variables for this type of event
			gameEvent->type = SHOOT_EVENT;
			gameEvent->lastDirect = shootEvent->getLastDirection();
		}
		else if (theEvent->getType() == DIE_EVENT)
		{
			//Needed variables for this type of event
			gameEvent->type = DIE_EVENT;
		}
		else if (theEvent->getType() == SPAWN_EVENT)
		{
			//Needed variables for this type of event
			gameEvent->type = SPAWN_EVENT;
		}

		delete theEvent; // Deleting the pointer
		gs->peer->Send((char*)(gameEvent), sizeof(GameEvent), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->peer->GetMyGUID(), true);
	}
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