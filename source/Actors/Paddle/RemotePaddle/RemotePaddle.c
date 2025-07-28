/*
 * VUEngine Showcase
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <Body.h>
#include <CommunicationManager.h>
#include <KeypadManager.h>
#include <Messages.h>

#include "RemotePaddle.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

mutation class RemotePaddle;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' MACROS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define REMOTE_NO_COMMAND					 0x00
#define REMOTE_SEND_USER_INPUT				 0xAB

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DATA
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef struct RemotePlayerData
{
	uint32 command;

	// Hold key(s)
	uint16 holdKey;

} RemotePlayerData;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool RemotePaddle::handlePropagatedMessage(int32 message)
{
	switch(message)
	{
		case kMessageKeypadHoldDown:
		{
			if(!isDeleted(this->body))
			{
				UserInput userInput = KeypadManager::getUserInput();

				RemotePaddle::syncWithRemote(this, &userInput);
			}

			return false;
		}
	}

	return false;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

uint32 RemotePaddle::getCommunicationCommand(uint32 message)
{
	switch(message)
	{
		case kMessageVersusModeSendInput:

			return REMOTE_SEND_USER_INPUT;
	}

	return REMOTE_NO_COMMAND;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool RemotePaddle::isMessageValid(uint32 message, uint8 command)
{
	return RemotePaddle::getCommunicationCommand(this, message) == command;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void RemotePaddle::syncWithRemote(const UserInput* userInput)
{
	/*
	 * This call will sync both systems. The approach in this example is to get both
	 * systems to follow the same code paths and only transmit the user input. So,
	 * both are at the end of each frame in the same state. It is possible to run
	 * the game in one and send the data to the other so this only shows it.
	 */
	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		/*
		* A command is used to verify that the received message and the data
		* are valid.
		*/
		uint8 command = RemotePaddle::getCommunicationCommand(this, kMessageVersusModeSendInput);

		/*
		* This is the struct that we are going to send down the link port.
		*/
		RemotePlayerData remotePlayerData;

		remotePlayerData.command = command;
		remotePlayerData.holdKey = userInput->holdKey;

		RemotePaddle::transmitData(this, kMessageVersusModeSendInput, (BYTE*)&remotePlayerData, sizeof(remotePlayerData));
	}
		
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void RemotePaddle::transmitData(uint32 messageForRemote, BYTE* data, uint32 dataBytes)
{
	uint32 receivedMessage = kMessageVersusModeDummy;
	const RemotePlayerData* remotePlayerData = NULL;

	/*
	 * Data will be sent sychroniously. This means that if the cable is disconnected during
	 * transmission, the mutator is undefined.
	 */
	do
	{
		/*
		 * Data transmission can fail if there was already a request to send data.
		 */
		if(!CommunicationManager::sendAndReceiveData(CommunicationManager::getInstance(), messageForRemote, data, dataBytes))
		{
			/*
			 * In this case, simply cancel all communications and try again. This supposes
			 * that there are no other calls that could cause a race condition.
			 */
			CommunicationManager::cancelCommunications(CommunicationManager::getInstance());
		}

		/*
		 * Every transmission sends a control message and then the data itself.
		 */
		receivedMessage = CommunicationManager::getReceivedMessage(CommunicationManager::getInstance());
		remotePlayerData = (const RemotePlayerData*)CommunicationManager::getReceivedData(CommunicationManager::getInstance());
	}
	/*
	 * The validity of the message is based on the command that was received
	 */
	while(!RemotePaddle::isMessageValid(this, receivedMessage, remotePlayerData->command));

	RemotePaddle::processReceivedMessage(this, messageForRemote, receivedMessage, remotePlayerData);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void RemotePaddle::processReceivedMessage(uint32 messageForRemote, uint32 receivedMessage, const RemotePlayerData* remotePlayerData)
{
	/*
	 * When both systems send the same message, they are in sync. If the received
	 * message differs from what I've sent, then update appropriately the message
	 * that I will send in the next cycle.
	 */
	switch(receivedMessage)
	{
		case kMessageVersusModeSendInput:

			if(kMessageVersusModeSendInput == messageForRemote)
			{

				fixed_t forceMagnitude = 0;
				
				if(0 != (K_LU & remotePlayerData->holdKey))
				{
					forceMagnitude = -__FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
				}
				else if(0 != (K_LD & remotePlayerData->holdKey))
				{
					forceMagnitude = __FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
				}

				Vector3D force = {0, forceMagnitude, 0};

				RemotePaddle::applyForce(this, &force, true);
			}

			break;
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
