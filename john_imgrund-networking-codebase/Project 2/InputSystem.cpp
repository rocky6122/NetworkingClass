//Created by John Imgrund
//Last Modified: 9/7/18

#include <SDL.h>

#include "InputSystem.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"

#include "PlayerMoveToMessage.h"
#include "PlayerExitMessage.h"
#include "UnitDeletionMessage.h"
#include "UnitAdditionMessage.h"
#include "FlockingCohesionWeightMessage.h"
#include "FlockingSeparationWeightMessage.h"
#include "FlockingVelocityMatchingWeightMessage.h"
#include "SaveLoadMessage.h"


InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}


//Processes all inputs on a given frame and attempts to execute all inputs
void InputSystem::processInputs()
{
	SDL_PumpEvents();
	//Mouse Inputs

	//int x, y;
	
	//if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	//{
	//	Vector2D pos(x, y);
	//	GameMessage* pMessage = new PlayerMoveToMessage(pos);
	//	MESSAGE_MANAGER->addMessage(pMessage, 0);
	//}

	//Keyboard Inputs
	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		//mShouldExit = true;

		GameMessage* pMessage = new PlayerExitMessage(true);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_D]) //If D is pressed than delete a random unit
	{
		GameMessage* pMessage = new UnitDeletionMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);					
	}
	else if (state[SDL_SCANCODE_A]) //Adds a random unit that wanders and chases the player
	{
		//MODFIED FOR ASSIGNMENT 3 BY JOHN IMGRUND

		for (int i = 0; i < 10; i++) // Creates 10 'Boid' as per assignment requirements
		{
			GameMessage* pMessage = new UnitAdditionMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
	}
	else if (state[SDL_SCANCODE_KP_7])
	{
		GameMessage* pMessage = new FlockingCohesionWeightMessage(-0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_KP_9])
	{
		GameMessage* pMessage = new FlockingCohesionWeightMessage(0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_KP_4])
	{
		GameMessage* pMessage = new FlockingSeparationWeightMessage(-0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_KP_6])
	{
		GameMessage* pMessage = new FlockingSeparationWeightMessage(0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_KP_1])
	{
		GameMessage* pMessage = new FlockingVelocityMatchingWeightMessage(-0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_KP_3])
	{
		GameMessage* pMessage = new FlockingVelocityMatchingWeightMessage(0.01f);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_S])
	{
		GameMessage* pMessage = new SaveLoadMessage(false);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (state[SDL_SCANCODE_L])
	{
		GameMessage* pMessage = new SaveLoadMessage(true);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	
}