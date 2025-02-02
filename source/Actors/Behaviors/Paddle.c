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

#include <Box.h>
#include <InGameTypes.h>
#include <Messages.h>
#include <VUEngine.h>

#include "Paddle.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' MACROS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define VERTICAL_FORCE	 	__I_TO_FIX10_6(3)

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
	/*
	switch(message)
	{
		case kMessagePongResetPositions:

			if(!Pong::isVersusMode(Pong::getInstance()))
			{
				Paddle::stopMovement(this, __ALL_AXIS);
				Vector3D localPosition = this->localTransformation.position;
				localPosition.y = 0;
				Paddle::setLocalPosition(this, &localPosition);
			}
			break;
	}
*/
	/* Returning false allows any other actor interested in the message to process it,
	 * returning true will make that the propagation stops here
	 */

	return false;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Paddle::moveTowards(NormalizedDirection direction)
{
	Vector3D force = 
	{
		0,
		__FIX10_6_MULT(VERTICAL_FORCE, __I_TO_FIX10_6(direction.y)),
		0
	};

	Paddle::applyForce(this, &force, true);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
