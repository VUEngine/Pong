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
	NormalizedDirection normalizedDirection = {0, 0, 0};

	if(this->transformation.position.x < 0)
	{
		switch(message)
		{
			case kMessageKeypadHoldLeftUp:
			{
				normalizedDirection.y = __UP;
				break;
			}

			case kMessageKeypadHoldLeftDown:
			{
				normalizedDirection.y = __DOWN;
				break;
			}
		}
	}
	else
	{
		switch(message)
		{
			case kMessageKeypadHoldRightUp:
			{
				normalizedDirection.y = __UP;
				break;
			}

			case kMessageKeypadHoldRightDown:
			{
				normalizedDirection.y = __DOWN;
				break;
			}
		}
	}


	if(0 != normalizedDirection.y)
	{
		Paddle::moveTowards(this, normalizedDirection);
		return true;
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
