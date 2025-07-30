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
#include <InGameTypes.h>
#include <Messages.h>
#include <RumbleEffects.h>
#include <RumbleManager.h>
#include <SoundManager.h>
#include <Sounds.h>

#include "Disk.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

mutation class Disk;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Disk::handlePropagatedMessage(int32 message)
{
	switch(message)
	{
		case kMessageStartGame:
		{
			Disk::resetPosition(this);
			Disk::startMoving(this);
			return false;
		}

		case kMessageVersusModePlayer1:
		{
			Disk::mutateMethod(mustSychronize, Disk::mustNotSychronize);
			Disk::resetPosition(this);
			return false;
		}

		case kMessageVersusModePlayer2:
		{
			Disk::resetPosition(this);
			return false;
		}
	}

	return false;
}

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
        // Intended fall through

		case kTypeWall:
		{
			SoundManager::playSound(&BounceSoundSpec,  NULL, kSoundPlaybackNormal, NULL);
			RumbleManager::startEffect(&BounceRumbleEffectSpec);
		}
		break;
	}

	return returnValue;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Disk::update()
{
	CommunicationManager communicationManager = CommunicationManager::getInstance();

	if(!CommunicationManager::isConnected(communicationManager))
	{
		return;
	}

	if
	(
		!CommunicationManager::sendAndReceiveData
		(
			communicationManager, Disk::getClass(), 
			(BYTE*)&this->transformation.position, sizeof(this->transformation.position)
		)
	)
	{
		return;
	}

	if(Disk::getClass() == CommunicationManager::getReceivedMessage(communicationManager))
	{
		if(Disk::mustSychronize(this))
		{
			Disk::stopMovement(this, __ALL_AXIS);

			Disk::setPosition(this, (const Vector3D*)CommunicationManager::getReceivedData(communicationManager));
		}
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Disk::resetPosition()
{
	Disk::stopMovement(this, __ALL_AXIS);
	Vector3D localPosition = this->localTransformation.position;
	localPosition.x = 0;
	localPosition.y = 0;
	Disk::setLocalPosition(this, &localPosition);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Disk::startMoving()
{
	int16 angle = (Math::random(Math::randomSeed(), 128) - 64) >> 1;

	Vector3D velocity = 
	{
		__FIXED_MULT(Body::getMaximumSpeed(this->body), __FIX7_9_TO_FIXED(__COS(angle))),
		__FIXED_MULT(Body::getMaximumSpeed(this->body), __FIX7_9_TO_FIXED(__SIN(angle))),
		0
	};

	if(50 > Math::random(Math::randomSeed(), 100))
	{
		velocity.x = -velocity.x;
	}

	Disk::setVelocity(this, &velocity, false);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Disk::mustSychronize()
{
	return true;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Disk::mustNotSychronize()
{
	return false;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

