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
	if(!CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		return;
	}

	uint32 receivedMessage = kMessageVersusModeDummy;
	uint16 receivedHoldKey = __KEY_NONE;

	/*
	 * Data will be sent sychroniously. This means that if the cable is disconnected during
	 * transmission, the mutator is undefined.
	 */
	do
	{
		/*
		 * Data transmission can fail if there was already a request to send data.
		 */
		if
		(
			!CommunicationManager::sendAndReceiveData
			(
				CommunicationManager::getInstance(), 
				kMessageVersusModeSendInput, (BYTE*)&holdKey, sizeof(holdKey)
			)
		)
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
		receivedHoldKey = *(const uint16*)CommunicationManager::getReceivedData(CommunicationManager::getInstance());
	}
	/*
	 * Keep sending the data until the received message matches the sent one
	 */
	while(kMessageVersusModeSendInput != receivedMessage);
	
	RemotePaddle::move(this, receivedHoldKey);
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
