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

#include "Paddle.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

mutation class Paddle;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Paddle::handlePropagatedMessage(int32 message)
{
	switch(message)
	{
		case kMessageKeypadHoldDown:
		{
			NormalizedDirection normalizedDirection = {0, 0, 0};

			UserInput userInput = KeypadManager::getUserInput();

			if(this->transformation.position.x < 0)
			{
				if(0 != (K_LU & userInput.holdKey))
				{
					normalizedDirection.y = __UP;
				}
				else if(0 != (K_LD & userInput.holdKey))
				{
					normalizedDirection.y = __DOWN;
				} 
			}
			else
			{
				if(0 != (K_RU & userInput.holdKey))
				{
					normalizedDirection.y = __UP;
				}
				else if(0 != (K_RU & userInput.holdKey))
				{
					normalizedDirection.y = __DOWN;
				} 
			}

			if(0 != normalizedDirection.y)
			{
				Paddle::moveTowards(this, normalizedDirection);
			}

			break;
		}
	}


	return false;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Paddle::moveTowards(NormalizedDirection direction)
{
	if(isDeleted(this->body))
	{
		return;
	}

	Vector3D force = 
	{
		0,
		__FIX10_6_MULT
		(
			__FIXED_MULT
			(
				Body::getMass(this->body), Body::getMaximumSpeed(this->body)
			),
			__I_TO_FIX10_6(direction.y)
		),
		0
	};

	Paddle::applyForce(this, &force, true);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
