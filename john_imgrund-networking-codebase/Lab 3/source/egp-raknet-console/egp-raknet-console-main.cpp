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
//EGP-405
//Lab 3
//10/4/2018
//This file was modified by John Imgrund and Parker Stazkiewicz with permission from author

#include "game.h"

// entry function
int main(int const argc, char const *const *const argv)
{
	GameState theGameState[1] = { 0 };
	GameInformation theGameInfo[1] = { 0 };
	

	// global peer settings for this app
	int isHost = -1;

	getUserName(theGameState);

	do {
		// create and return instance of peer interface
		RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
		theGameState->peer = peer;

		// ask user for peer type
		isHost = lobbyState();

		// start networking
		if (isHost == 0)		// JOIN GAME
		{
			joinGame(theGameState);
		}
		else if (isHost == 1)	// HOST GAME
		{
			hostGame(theGameState);
		}
		else //Prepares to quit
		{
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


		while (theGameState->isPlaying)
		{
			handleNetworking(theGameState);

			handleInput(theGameState, theGameInfo);

			handleUpdate(theGameState, theGameInfo);

			handleDisplay(theGameState, theGameInfo);
		}

		// shut down networking by destroying peer interface instance
		RakNet::RakPeerInterface::DestroyInstance(peer);

	} while (!theGameState->isLeaving);

	// exit
	return 0;
}

