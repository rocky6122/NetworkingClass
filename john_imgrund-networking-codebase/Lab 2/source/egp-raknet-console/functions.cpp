//Team Member Names: John (82-1019344) Parker Stazkiewicz (82-0967451)
//EGP-405-01
//Lab 2
//9/13/18
//“We certify that this work is entirely our own.The assessor of this project 
//may reproduce this project and provide copies to other academic staff, 
//and/or communicate a copy of this project to a plagiarism - checking service, 
//which may retain a copy of the project on its database.” 

#include "functions.h"

int lobbyState()
{
	char str[512];
	int temp;

	do
	{
		printf("(J)oin a lobby, (H)ost a lobby, or (Q)uit?\n");
		fgets(str, 512, stdin);

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

RakNet::RakNetGUID getGUID(UserData * array, char targetName[15], int arraySize)
{
	RakNet::RakNetGUID temp;

	char name[15];
	char testName[15];

	strcpy(name, targetName);


	for(int i = 0; i < arraySize; i++)
	{
		strcpy(testName, array[i].userName);

		if (strcmp(testName,name) == 0)
		{
			temp = array[i].userGUID;
			break;
		}
	}

	return temp;
}