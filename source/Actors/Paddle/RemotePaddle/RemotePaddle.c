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
			break;
			if(!isDeleted(this->body))
			{
				UserInput userInput = KeypadManager::getUserInput();

				fixed_t forceMagnitude = 0;
				
				if(0 != (K_LU & userInput.holdKey))
				{
					forceMagnitude = -__FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
				}
				else if(0 != (K_LD & userInput.holdKey))
				{
					forceMagnitude = __FIXED_MULT(Body::getMass(this->body), Body::getMaximumSpeed(this->body));
				}

				Vector3D force = {0, forceMagnitude, 0};

				RemotePaddle::applyForce(this, &force, true);
			}

			return true;
		}
	}

	return false;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
