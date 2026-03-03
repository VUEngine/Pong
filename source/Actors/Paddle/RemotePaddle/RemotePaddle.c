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
#include <Communications.h>
#include <Keypad.h>
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
			UserInput userInput = Keypad::getUserInput();

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
	if(Communications::isConnected())
	{
		if(
			Communications::sendAndReceiveData((uint32)RemotePaddle::getClass(), (uint8*)&holdKey, sizeof(holdKey))
		)
		{
			if((uint32)RemotePaddle::getClass() == Communications::getReceivedMessage())
			{
				RemotePaddle::move(this, *(const uint16*)Communications::getReceivedData());
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
