/*
	RakNet Console Application
	Prepared by Dan Buckstein
	September 5, 2018

	Simple RakNet application following official tutorials.

	****TO-DO: add your own implementations and comments

	Resources: 
	http://www.jenkinssoftware.com/raknet/manual/tutorialsample1.html
	http://www.jenkinssoftware.com/raknet/manual/tutorialsample2.html
	http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html

	http://www.raknet.net/raknet/manual/creatingpackets.html
	http://www.raknet.net/raknet/manual/receivingpackets.html
	http://www.raknet.net/raknet/manual/sendingpackets.html
	http://www.raknet.net/raknet/manual/timestamping.html

	Read them for the secrets to success!
	E.g. pointers on pointers ;)
*/

//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/13/18
//Below is a heavily modified version of the original source code provided. We certify that all changes are our own.

#include "functions.h"

// entry function
int main(int const argc, char const *const *const argv)
{
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];
	char str2[bufferSz];

	const unsigned int NAME_SIZE = 512;

	// create and return instance of peer interface
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

	// global peer settings for this app
	int isHost = -1;
	unsigned short serverPort;
	unsigned int maxClients;

	//Bools for while logic
	bool isChatting = false;
	bool isExiting = false;

	//Personal Variables
	char userName[NAME_SIZE];
	UserData* clientData;
	int currentClientNum = 0;
	RakNet::RakNetGUID host_id;


	printf("Please enter a username for this chat instance (15 characters or less):\n");
	fgetstr(userName, NAME_SIZE, stdin);


	do {
		// ask user for peer type
		isHost = lobbyState();

		// start networking
		if (isHost == 0)		// JOIN CHATROOM
		{
			//Finds the chatroom to join
			RakNet::SocketDescriptor sd;
			peer->Startup(1, &sd, 1);
			peer->Startup(1, &sd, 1);

			printf("Enter server IP or hit enter for 127.0.0.1\n");
			fgets(str, bufferSz, stdin);

			if (str[0] == '\n')
			{
				strcpy(str, "127.0.0.1");
			}

			printf("Enter Server Port Number: \n");
			fgets(str2, bufferSz, stdin);
			serverPort = atoi(str2);

			printf("Joining the chatroom.\n");
			peer->Connect(str, serverPort, 0, 0);
			isChatting = true;

			printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n /help to display these commands.\n");
		}
		else if(isHost == 1)	// HOST CHATROOM
		{
			printf("Enter maximum number of people who can join the chatroom: \n");
			fgets(str, bufferSz, stdin);

			maxClients = atoi(str);

			printf("Enter Server Port Number: \n");
			fgets(str2, bufferSz, stdin);

			serverPort = atoi(str2);
			RakNet::SocketDescriptor sd(serverPort, 0);
			peer->Startup(maxClients, &sd, 1);

			clientData = new UserData[maxClients];

			// We need to let the server accept incoming connections from the clients
			printf("Starting the Chatroom.\n");
			peer->SetMaximumIncomingConnections(maxClients);
			isChatting = true;

			printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n Press ` to get client list.\n /help to display these commands.\n");
		}
		else //Prepares to quit
		{
			isChatting = false;
			isExiting = true;
		}

		RakNet::Packet *packet;

		while (isChatting)
		{

			if (_kbhit()) //Looks for inputs
			{
				char input = _getch(); //gets input if there is any

				if (input == 13)
				{
					//designates that the user can type a message
					printf("[%s]: ", userName);
					char message[512];
					fgetstr(message, bufferSz, stdin);

					if (message[0] == '/') //User is attempting to execute a command
					{
						if (message[1] == 'h' || message[1] == 'H') //Help command
						{
							if (isHost == 1)
							{
								printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n Press ` to get client list.\n");
							}
							else
							{
								printf("Commands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n");
							}
						}
						else if (message[1] == 'w' || message[1] == 'W') //Private  Messages command
						{
							char targetName[15]; //Target user of private message
							char newMessage[512];

							int counter = 0;
							int targetCounter = 0;
							while (message[counter] != 32)
							{
								counter++;
							}
							
							counter++;

							//Finds the target users name and adds it to targetName
							do
							{
								targetName[targetCounter] = message[counter];
								targetCounter++;
								counter++;
							} while (message[counter] != 32);

							targetName[targetCounter] = '\0';

							targetCounter = 0; //resetting target counter to save space
							counter++;

							//Adds the rest of the message into a new Message char
							while (counter < 512)
							{
								newMessage[targetCounter] = message[counter];
								targetCounter++;
								counter++;
							}

							if (isHost == 0) //Client
							{
								SendMessageRequest msgRequest[1];
								msgRequest->userGUID = peer->GetMyGUID();
								strcpy(msgRequest->userName, userName);
								strcpy(msgRequest->message, newMessage);
								strcpy(msgRequest->targetName, targetName);

								peer->Send((char*)(msgRequest), sizeof(SendMessageRequest), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host_id, false);
							}
							else //Host
							{
								MessageDelivery delivery[1];
								strcpy(delivery->userName, userName);
								strcpy(delivery->message, newMessage);
								delivery->isPrivate = true;

								peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, getGUID(clientData, targetName, maxClients), false);
							}
						}
						else //No command recognized
						{
							printf("Command Entered does not exist. For list of commands type : /help\n");
						}
					}
					else //Normal Message
					{
						if (isHost == 1) //Host
						{
							MessageDelivery delivery[1];
							strcpy(delivery->userName, userName);
							strcpy(delivery->message, message);
							


							peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host_id, true);

						}
						else //Client
						{
							SendMessageRequest msgRequest[1];
							msgRequest->userGUID = peer->GetMyGUID();
							strcpy(msgRequest->userName, userName);
							strcpy(msgRequest->message, message);
							msgRequest->targetName[0] = '\0';

							peer->Send((char*)(msgRequest), sizeof(SendMessageRequest), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host_id, false);
						}
					}
				}
				else if (input == 27) //User quitting
				{
					SendMessageRequest msgRequest[1];
					msgRequest->userGUID = peer->GetMyGUID();
					strcpy(msgRequest->userName, userName);
					strcpy(msgRequest->message, "has left the chatroom.\n");
					msgRequest->targetName[0] = '\0';

					peer->Send((char*)(msgRequest), sizeof(SendMessageRequest), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host_id, false);

					isChatting = false;
				}
				else if (input == 96) //Host Only: Outputs all sevrer members and info
				{
					if (isHost == 1)
					{
						for (int i = 0; i < maxClients; i++)
						{
							printf("Name: %s\n", clientData[i].userName);
							printf("IP Address: %s\n", clientData[i].address.ToString());
							printf("Time connected: %d\n", clientData[i].timeStamp); //PROBLEM HERE
							printf("----------\n");
						}
					}
				}
			}


			for (
				packet = peer->Receive();
				packet;
				peer->DeallocatePacket(packet), packet = peer->Receive()
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
						strcpy(data->userName, userName);
						data->userGUID = peer->GetMyGUID();
						data->address = peer->GetSystemAddressFromGuid(peer->GetMyGUID());
						data->timeStamp = RakNet::GetTime();
						peer->Send((char*)(data), sizeof(UserData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					break;
				case ID_NEW_INCOMING_CONNECTION:
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					if (isHost) {
						printf("A client has disconnected.\n");
					}
					else {
						printf("We have been disconnected.\n");
					}
					break;
				case ID_CONNECTION_LOST:
					if (isHost) {
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

						host_id = MSG->hostID;

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


						peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, getGUID(clientData, MSG->targetName, maxClients), false);
					}
					else //Public Message
					{
						MessageDelivery delivery[1];
						strcpy(delivery->userName, MSG->userName);
						strcpy(delivery->message, MSG->message);

						printf("[%s]: ", MSG->userName);
						printf("%s\n", MSG->message);

						peer->Send((char*)(delivery), sizeof(MessageDelivery), HIGH_PRIORITY, RELIABLE_ORDERED, 0, MSG->userGUID, true);
					}
				}
					break;

				case SEND_MESSAGE_DELIVERY:
				{
					const MessageDelivery *MSG = (MessageDelivery *)packet->data;

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
				}
				break;

				case SEND_USER_DATA:
					{

					if (currentClientNum == 0) //Host (Sets host data into the array on first run through)
					{
						UserData hostData;
						strcpy(hostData.userName, userName);
						hostData.userGUID = peer->GetMyGUID();
						hostData.address = peer->GetSystemAddressFromGuid(peer->GetMyGUID());
						hostData.timeStamp = RakNet::GetTime();
						clientData[currentClientNum] = hostData;

						currentClientNum++;
					}

						UserData *userData = (UserData *)packet->data;

						clientData[currentClientNum] = *userData;
						currentClientNum++;

						printf("%s has joined the chatroom.\n", userData->userName);

						ServerMessage message[1];
						message->typeID = ID_SERVER_MESSAGE_1;
						message->hostID = peer->GetMyGUID();
						strcpy(message->userName, userData->userName);
						strcpy(message->message, " has joined the chat room.\n");

						peer->Send((char*)(message), sizeof(ServerMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
					}
					break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
			}
		}
	} while (!isExiting);
		
	// shut down networking by destroying peer interface instance
	RakNet::RakPeerInterface::DestroyInstance(peer);


	// exit
	return 0;
}

