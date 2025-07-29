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
		case kMessageVersusModePlayer1:
		case kMessageVersusModePlayer2:
		{
			Disk::resetPosition(this);
			Disk::startMoving(this);
			return false;
		}

		case kMessageKeypadHoldDown:
		{
			Disk::sychronize(this);
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

void Disk::ready(bool recursive)
{
	Base::ready(this, recursive);

	Disk::startMoving(this);
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

void Disk::sychronize()
{
	if(!CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		return;
	}

	typedef struct RemoteDiskData
	{
		Vector3D position;
		Vector3D velocity;

	} RemoteDiskData;

	uint32 receivedMessage = kMessageVersusModeDummy;
	RemoteDiskData remoteDiskData = 
	{
		*Body::getPosition(this->body),
		*Body::getVelocity(this->body)
	};

	do
	{
		if
		(
			!CommunicationManager::sendAndReceiveData
			(
				CommunicationManager::getInstance(), kMessageVersusModeSendInput, (BYTE*)&remoteDiskData, sizeof(remoteDiskData)
			)
		)
		{
			CommunicationManager::cancelCommunications(CommunicationManager::getInstance());
		}

		receivedMessage = CommunicationManager::getReceivedMessage(CommunicationManager::getInstance());
		remoteDiskData = *(const RemoteDiskData*)CommunicationManager::getReceivedData(CommunicationManager::getInstance());
	}
	while(kMessageVersusModeSendInput != receivedMessage);

	Vector3D averageBodyPosition = Vector3D::intermediate(*Body::getPosition(this->body), remoteDiskData.position);
	Vector3D averageBodyVelocity = Vector3D::intermediate(*Body::getVelocity(this->body), remoteDiskData.velocity);

	Body::setPosition(this->body, &averageBodyPosition, Entity::safeCast(this));
	Body::setVelocity(this->body, &averageBodyVelocity);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
