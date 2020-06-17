//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/22/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "MancalaNetworked.h"

void setUpGame(GameState* gs)
{
	//Gets ready to start the game by allocating the neccesary game info
	if (gs->currentClientNum == 2 && !gs->gameStarted)
	{
		gs->gameStarted = 1;
		gs->myTurn = 1;
		gs->updateRender = 1;

		//Assigns the player names
		strcpy(gs->mData.player0Name, gs->userName);
		strcpy(gs->mData.player1Name, gs->clientData[1].userName);

		//Set starting scores to 0
		gs->mData.scores[0] = 0;
		gs->mData.scores[1] = 0;

		//Sets the starting board
		int playerBuffer[16] = {
			4, 4, 4, 4, 4, 4, 0, 24,	// player 0: starts at element 0
			4, 4, 4, 4, 4, 4, 0, 24,	// player 1: starts at element 8
		};

		memcpy(gs->mData.gameBoard, playerBuffer, sizeof(playerBuffer));

		//gs->mData.playerCount[0] = gs->mData.gameBoard + 0;
		//gs->mData.playerCount[1] = gs->mData.gameBoard + 8;
	}
}

void handleTurn(GameState* gs)
{
	int activePlayer = 0, otherPlayer = 1, cupIndex = 0, winner = -1;
	int turnResult, playerIndexResult[1], cupIndexResult[1], *stealResult[1];

	int* playerCount[2];
	playerCount[0] = gs->mData.gameBoard + 0;
	playerCount[1] = gs->mData.gameBoard + 8;

	bool stop = false;

	if (playerCount[0][7] && playerCount[1][7] && playerCount[0][6] < 25 && playerCount[1][6] < 25) //GAME IS STILL IN PLAY
	{
		while (!stop && playerCount[0][7] && playerCount[1][7] && playerCount[0][6] < 25 && playerCount[1][6] < 25)
		{
			//Loop asks player to pick a cup to move
			do
			{
				printf("Pick a cup with stones (1-6): ");
				cupIndex = getchar() - 48;
				while (getchar() != '\n');

				if (gs->isServer)
				{
					turnResult = cupIndex > 0 && cupIndex <= 6 ? playerCount[activePlayer][--cupIndex] : 0;
				}
				else
				{
					turnResult = cupIndex > 0 && cupIndex <= 6 ? playerCount[otherPlayer][--cupIndex] : 0;
				}
			} while (turnResult == 0);


			// take a turn
			if (gs->isServer)
			{
				turnResult = MancalaTurn(playerCount, activePlayer, cupIndex, playerIndexResult, cupIndexResult, stealResult);
			}
			else
			{
				turnResult = MancalaTurn(playerCount, otherPlayer, cupIndex, playerIndexResult, cupIndexResult, stealResult);
			}


			// handle steal if landed in empty cup with stones across from it
			if (*stealResult && **stealResult)
			{
				printf("Landed in empty cup, steal\? 0 = no, other = yes: ");
				cupIndex = getchar() - 48;
				while (getchar() != '\n');
				if (cupIndex)
				{
					if (gs->isServer)
					{
						playerCount[activePlayer][*cupIndexResult] += **stealResult;
					}
					else
					{
						playerCount[otherPlayer][*cupIndexResult] += **stealResult;
					}
					
					**stealResult = 0;
				}
			}

			if (turnResult == 0)
			{
				stop = true;
			}
			else
			{
				system("cls");
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

			// update counts
			cupIndex = MancalaUpdateTotal(playerCount);

			if (cupIndex != 48)
				printf("\n failed checksum");
		}
	}
	else //GAME IS OVER
	{
		// end of game, put any leftovers in that player's mancala
		playerCount[0][6] += playerCount[0][7];
		playerCount[1][6] += playerCount[1][7];

		//Sends scores into gamestate
		gs->mData.scores[0] = playerCount[0][6];
		gs->mData.scores[1] = playerCount[1][6];

		gs->mData.winner = (playerCount[0][6] > playerCount[1][6] ? 0 : playerCount[0][6] < playerCount[1][6] ? 1 : -1);
		gs->gameStarted = 0;
	}
}

// print board
void MancalaNetworkPrintBoard(int gameBoard[], char topPlayer[15], char bottomPlayer[15], int topIndex, int bottomIndex)
{
	int value;
	int i;
	printf("\n");
	printf("\n %s\'s cups", topPlayer);
	printf("\n  Score");
	printf("\n     ______________________________ ");

	printf("\n    |");
	for (i = 6; i >= 0; --i)
		printf((value = gameBoard[topIndex+i]) ? "%2d  " : "    ", value);
	printf("  |");

	printf("\n    |______________________________|");

	printf("\n    |  ");
	for (i = 0; i <= 6; ++i)
		printf((value = gameBoard[bottomIndex+i]) ? "  %2d" : "    ", value);
	printf("|");

	printf("\n    |______________________________|");
	printf("\n  #       1   2   3   4   5   6   SCORE");
	printf("\n %s\'s cups", bottomPlayer);
	printf("\n");
}
