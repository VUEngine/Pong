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
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool RemotePaddle::handlePropagatedMessage(int32 message)
{
	switch(message)
	{
		case kMessageKeypadHoldDown:
		{
			UserInput userInput = KeypadManager::getUserInput();

			RemotePaddle::transmitData(this, userInput.holdKey);

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

void RemotePaddle::transmitData(uint16 holdKey)
{
	CommunicationManager communicationManager = CommunicationManager::getInstance();

	if(CommunicationManager::isConnected(communicationManager))
	{
		if(
			CommunicationManager::sendAndReceiveData
			(
				communicationManager, (uint32)RemotePaddle::getClass(), (BYTE*)&holdKey, sizeof(holdKey)
			)
		)
		{
			if((uint32)RemotePaddle::getClass() == CommunicationManager::getReceivedMessage(communicationManager))
			{
				RemotePaddle::move(this, *(const uint16*)CommunicationManager::getReceivedData(communicationManager));
			}
		}
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void RemotePaddle::move(uint16 holdKey)
{
	fixed_t forceMagnitude = 0;
	
	if(0 != (K_LU & holdKey))
	{
		forceMagnitude = -__FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
	}
	else if(0 != (K_LD & holdKey))
	{
		forceMagnitude = __FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
	}

	Vector3D force = {0, forceMagnitude, 0};

	RemotePaddle::applyForce(this, &force, true);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
