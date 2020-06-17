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

//Team Member Names: John Imgrund (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/13/18
//This file was modified by John Imgrund and Parker Stazkiewicz with permission from author

#include "functions.h"
#include "game.h"
#include "networking.h"
#include "mancalaFunctions.h"



// entry function
int main(int const argc, char const *const *const argv)
{
	GameState theGameState[1] = { 0 };

	do {

		// Get whether game is local or online
		bool online;
		bool play = true;
		char input[512];

		while (true)
		{
			system("cls");
			printf("Do you want to play (L)ocal or (O)nline or (Q)uit?\n");
			fgetstr(input, 512, stdin);

			if (input[0] == 'L' || input[0] == 'l')
			{
				online = false;
				break;
			}
			else if (input[0] == 'O' || input[0] == 'o')
			{
				online = true;
				break;
			}
			else if (input[0] == 'Q' || input[0] == 'q')
			{
				online = false;
				play = false;
				theGameState->isRunning = 0;
				break;
			}
		}

		// Local Game
		if (!online && play)
		{
			playLocalMancala(theGameState);
		}
		// Online Functionality
		else if (online && play)
		{
			// create and return instance of peer interface
			RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
			theGameState->peer = peer;

			// global peer settings for this app
			int isHost = -1;

			//Bools for while logic
			bool isChatting = false;
			bool isExiting = false;


			getUserName(theGameState);

			do {
				// ask user for peer type
				isHost = lobbyState();

				// start networking
				if (isHost == 0)		// JOIN CHATROOM
				{
					joinChatroom(theGameState);
				}
				else if (isHost == 1)	// HOST CHATROOM
				{
					hostChatroom(theGameState);
				}
				else //Prepares to quit
				{
					isChatting = false;
					isExiting = true;
					theGameState->isLeaving = 1;
				}

				// Hacky solution to problem:
				// This prevents GetAsyncKeyState from
				// using input from previous segments of code
				for (int key = 0; key < 256; key++)
				{
					for (int loop = 0; loop < 1000; loop++)
					{
						GetAsyncKeyState(key);
					}
				}


				while (theGameState->isChatting)
				{
					handleInput(theGameState);

					handleNetworking(theGameState);

					handleDisplay(theGameState);

					handleUpdate(theGameState);
				}

			} while (!theGameState->isLeaving);

			// shut down networking by destroying peer interface instance
			RakNet::RakPeerInterface::DestroyInstance(peer);

		}
	} while (theGameState->isRunning);

	// exit
	return 0;
}

