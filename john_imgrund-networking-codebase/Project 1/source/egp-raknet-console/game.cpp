//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "game.h"

void handleInput(GameState * gs)
{
	// PLAYER INPUT
	if (gs->isPlayer)
	{
		
	}
	// SPECTATOR INPUT
	else if (!gs->isPlayer) 
	{
		memcpy(gs->keyBoardState_0, gs->keyBoardState, sizeof(gs->keyBoardState));

		for (int i = 0; i < 256; i++)
		{
			gs->keyBoardState[i] = GetAsyncKeyState(i);
		}

		if (gs->isTyping)
		{
			bool shift = false;
			if (gs->keyBoardState[16])
			{
				shift = true;
			}


			if (gs->keyBoardState[8] && !gs->keyBoardState_0[8]) //BackSpace
			{
				if (gs->msgLength > 0) //Avoids array error
				{
					--gs->msgLength;
					gs->msgData.message[gs->msgLength] = '\0'; //Sets the 'deleted' char to null terminator
				}

				gs->updateRender = 1;
			}
			else //If it was a backspace we dont want to put that into the message as a random char or 'x1'
			{
				for (int i = 0; i < 256; i++) //Checks to see which key was pressed and adds it to the message
				{
					if (gs->keyBoardState[i] && !gs->keyBoardState_0[i])
					{
						//Since special characters do not work correctly with async we have to convert the special characters
						//To the correct decimal values. 191 SHOULD be the value when the '/' key is pressed
						//you can check http://cherrytree.at/misc/vk.htm for a list of the values.
						if (i == 191)
						{
							gs->msgData.message[gs->msgLength] = '/';
							gs->msgLength++;
						}
						else if (i == 13)
						{
							//DO NOTHING BECAUSE WE WANT TO WAIT NOW TO SEND IT
						}
						else if (i == 160 || i == 161 || i == 16)
						{
							// DO NOT ENTER SHIFT KEY
						}
						else
						{
							int offset = 0;
							if (i <= 90 && i >= 65)
							{
								if (!shift)
								{
									offset = 32;
								}

							}

							gs->msgData.message[gs->msgLength] = (char)(i + offset);
							gs->msgLength++;
						}

						// Set flag to render new input
						gs->updateRender = 1;
					}
				}
			}
		}

		if (gs->keyBoardState[13] && !gs->keyBoardState_0[13]) //ENTER KEY
		{
			if (gs->isTyping) //Message is ready to be sent
			{
				gs->send = 1;
				gs->isTyping = 0;
			}
			else //Starts a message
			{
				//gs->msgLength = 0;
				gs->isTyping = 1;

				for (int i = gs->msgLength; i >= 0; i--)
				{
					gs->msgData.message[i] = '\0';

					if (gs->msgLength > 0)
					{
						gs->msgLength--;
					}

				}
			}

			gs->updateRender = 1;
		}

		if (gs->keyBoardState[27] && !gs->keyBoardState_0[27]) //User quitting ESCAPE KEY
		{
			gs->isChatting = 0;
			gs->isTyping = 0;

			//Clears the screen to return to the 'main menu'
			system("cls");
		}

		if (gs->keyBoardState[192] && !gs->keyBoardState_0[192]) //Host Only: Outputs all server members and info
		{
			if (gs->isServer == 1)
			{
				for (unsigned int i = 0; i < gs->maxClients; i++)
				{
					printf("Name: %s\n", gs->clientData[i].userName);
					printf("IP Address: %s\n", gs->clientData[i].address.ToString());
					printf("Time connected: %d\n", gs->clientData[i].timeStamp);
					printf("----------\n");
				}
			}
		}

		if (gs->send)
		{
			parseMessage(gs);
		}
	}
}

void parseMessage(GameState * gs)
{
	if (gs->msgData.message[0] == '/') //User is attempting to execute a command
	{
		if (gs->msgData.message[1] == 'h' || gs->msgData.message[1] == 'H') //Help command
		{
			if (gs->isServer)
			{
				printf("\nCommands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n Press ` to get client list.\n");
			}
			else
			{
				printf("\nCommands are:\n ESC to leave the server.\n ENTER to begin a message and ENTER to send it.\n /w [name] [message] to send a private message.\n");
			}

			gs->send = 0; //No longer want to send a message because the command list is for local use
			gs->updateRender = 0;
		}
		else if (gs->msgData.message[1] == 'w' || gs->msgData.message[1] == 'W') //Private  Messages command
		{
			preparePrivateMessage(gs);
		}
		else //No command recognized
		{
			printf("\nCommand Entered does not exist. For list of commands type : /help\n");
			gs->send = 0;
			gs->updateRender = 0;
		}
	}
}

void preparePrivateMessage(GameState * gs)
{
	char targetName[15]; //Target user of private message
	char newMessage[512];

	int counter = 0;
	int targetCounter = 0;
	while (gs->msgData.message[counter] != 32)
	{
		counter++;
	}

	counter++;

	//Finds the target users name and adds it to targetName
	do
	{
		targetName[targetCounter] = gs->msgData.message[counter];
		targetCounter++;
		counter++;
	} while (gs->msgData.message[counter] != 32);

	targetName[targetCounter] = '\0';

	targetCounter = 0; //resetting target counter to save space
	counter++;

	//Adds the rest of the message into a new Message char
	while (counter < 512)
	{
		newMessage[targetCounter] = gs->msgData.message[counter];
		targetCounter++;
		counter++;
	}

	strcpy(gs->msgData.message, newMessage);
	strcpy(gs->msgData.targetName, targetName);
}

void handleUpdate(GameState * gs)
{
	if (gs->isPlayer)
	{
		if (gs->mData.winner == -1) //Runs while there is no winner
		{
			if (gs->myTurn)
			{
				//Takes turn
				handleTurn(gs);

				//Ends Turn
				gs->myTurn = 0;
				gs->updateRender = 1;
				gs->sendGameData = 1;
			}

			if (gs->isServer && !gs->gameStarted)
			{
				//Setup of game info
				setUpGame(gs);

			}
		}
	}
}

	


void handleDisplay(GameState *gs)
{
	//TO DO: Display Mancala
	//TO DO: Display Previous Messages

	if (gs->updateRender)
	{
		system("cls");

		if (gs->isPlayer)
		{
			if (gs->mData.winner != -1)
			{
				if (gs->mData.winner == 0)
				{
					printf("Winner is %s", gs->mData.player0Name);
				}
				else if (gs->mData.winner == 1)
				{
					printf("Winner is %s", gs->mData.player1Name);
				}

				//Drops the players back into the chatroom.
				gs->isPlayer = 0;
			}
			else
			{
				if (gs->myTurn)
				{
					if (gs->isServer)
					{
						MancalaPrintTitle();
						MancalaNetworkPrintBoard(gs->mData.gameBoard, gs->mData.player1Name, gs->mData.player0Name, 8, 0);
					}
					else
					{
						MancalaPrintTitle();
						MancalaNetworkPrintBoard(gs->mData.gameBoard, gs->mData.player0Name, gs->mData.player1Name, 0, 8);
					}
				}
				else
				{
					printf("Your oppenent is thinking...\n");
					if (gs->isServer)
					{
						printf("Messages being sent as of now:\n"); // delighter: allows the host to view messages sent while waiting for their turn.
					}
				}
			}
		}
		else
		{
			//Checks to see if there is a game in progress to spectate
			if (gs->mData.winner == -1)
			{
				//Checks to see if there is a game in progress to spectate
				if (gs->mData.player0Name[0] != NULL)
				{
					MancalaPrintTitle();
					MancalaNetworkPrintBoard(gs->mData.gameBoard, gs->mData.player1Name, gs->mData.player0Name, 8, 0);
				}
			}

			// Show backlog of messages
			for (int i = 0; i < gs->log.size(); i++)
			{
				if (gs->log[i].isPrivate)//Private Messages
				{
					printf("[%s(Whispered)]: ", gs->log[i].username);
					printf("%s\n", gs->log[i].message);
				}
				else //Public Messages
				{
					printf("[%s]: ", gs->log[i].username);
					printf("%s\n", gs->log[i].message);
				}
			}

			// New Message being typed
			if (gs->isTyping)
			{
				//designates that the user can type a message
				printf("[%s]: ", gs->userName);
				printf("%s", gs->msgData.message);
			}
		}

		gs->updateRender = 0;
	}
}


