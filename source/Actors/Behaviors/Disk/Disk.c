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
#include <GameEvents.h>
#include <InGameTypes.h>
#include <MessageDispatcher.h>
#include <Messages.h>
#include <Optics.h>
#include <ParticleSystem.h>
#include <PongManager.h>
//#include <RumbleEffects.h>
#include <RumbleManager.h>
#include <SoundManager.h>
//#include <Sounds.h>
#include <Telegram.h>
#include <Utilities.h>
#include <VUEngine.h>

#include "Disk.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' MACROS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define MAXIMUM_SPEED						   __I_TO_FIXED(2)

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

mutation class Disk;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' ATTRIBUTES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/*
 * This is used to generate a new angle each time that the ball starts to move.
 * For this to work, this has to be kept in sync between both VBs and is has to
 * persist between calls to startMovement.
 */
static uint32 _randomSeed = 0;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Disk::collisionStarts(const CollisionInformation* collisionInformation)
{
	ASSERT(collisionInformation->otherCollider, "Actor::collisionStarts: otherColliders");

	bool returnValue = Base::collisionStarts(this, collisionInformation);

	if(NULL == collisionInformation->otherCollider)
	{
		return returnValue;
	}

	Entity collidingEntity = Collider::getOwner(collisionInformation->otherCollider);

	switch(Entity::getInGameType(collidingEntity))
	{
		case kTypePaddle:
		{
			Vector3D velocity = *Body::getVelocity(this->body);

			fixed_t yDisplacement = this->transformation.position.y - Entity::getPosition(collidingEntity)->y;

			velocity.y += yDisplacement;

			Body::setVelocity(this->body, &velocity);
		}
		break;
	}

	return returnValue;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Disk::ready(bool recursive)
{
	Base::ready(this, recursive);

	Disk::startMovement(this);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Disk::startMovement()
{
	Disk::stopMovement(this, __ALL_AXIS);

	Body::setMaximumVelocity(this->body, (Vector3D){MAXIMUM_SPEED, MAXIMUM_SPEED, 0});

	int16 angle = 0;

	if(0 == _randomSeed)
	{
		_randomSeed = 7;
	}

	_randomSeed ^= _randomSeed << 13;
	_randomSeed ^= _randomSeed >> 17;
	_randomSeed ^= _randomSeed << 5;

	angle = Math::random(Math::randomSeed() + _randomSeed, 64) - 32;

	Vector3D velocity = 
	{
		__FIXED_MULT(MAXIMUM_SPEED, __FIX7_9_TO_FIXED(__COS(angle))),
		__FIXED_MULT(MAXIMUM_SPEED, __FIX7_9_TO_FIXED(__SIN(angle))),
		0
	};

	if(50 > Math::random(Math::randomSeed() + _randomSeed, 100))
	{
		velocity.x = -velocity.x;
	}

	Disk::setVelocity(this, &velocity, false);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
