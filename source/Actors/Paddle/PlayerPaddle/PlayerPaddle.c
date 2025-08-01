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
#include <RemotePaddle.h>

#include "PlayerPaddle.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

mutation class PlayerPaddle;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool PlayerPaddle::handlePropagatedMessage(int32 message)
{
	switch(message)
	{
		case kMessageVersusModePlayer1:
		{
			PlayerPaddle::resetPosition(this);
			return false;
		}

		case kMessageVersusModePlayer2:
		{
			PlayerPaddle::resetPosition(this);
			PlayerPaddle::mutateTo(this, RemotePaddle::getClass());
			return false;
		}

		case kMessageKeypadHoldDown:
		{
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

				PlayerPaddle::applyForce(this, &force, true);
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

void PlayerPaddle::resetPosition()
{
	PlayerPaddle::stopMovement(this, __ALL_AXIS);
	Vector3D localPosition = this->localTransformation.position;
	localPosition.y = 0;
	PlayerPaddle::setLocalPosition(this, &localPosition);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
