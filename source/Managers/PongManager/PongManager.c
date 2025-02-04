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

#include <string.h>

#include <GameEvents.h>
#include <Paddle.h>
#include <Printer.h>
#include <RumbleEffects.h>
#include <RumbleManager.h>
#include <SoundManager.h>
#include <Sounds.h>

#include "PongManager.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::constructor(Stage stage)
{
	// Always explicitly call the base's constructor
	Base::constructor();

	this->leftPaddle = NULL;
	this->rightPaddle = NULL;

	PongManager::getReady(this, stage);

	this->leftScore = 0;
	this->rightScore = 0;

	PongManager::printScore(this);

	Printer::addEventListener(Printer::getInstance(), ListenerObject::safeCast(this), kEventFontRewritten);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::destructor()
{
	this->leftPaddle = NULL;
	this->rightPaddle = NULL;

	// Allow a new construct	// Always explicitly call the base's destructor
	Base::destructor();
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool PongManager::onEvent(ListenerObject eventFirer __attribute__((unused)), uint16 eventCode)
{
	switch(eventCode)
	{
		case kEventFontRewritten:
		{
			PongManager::printScore(this);

			return true;
		}

		case kEventActorDeleted:
		{
			if(0 == strcmp(DISK_NAME, Disk::getName(eventFirer)))
			{
				SoundManager::playSound(&PointSoundSpec,  NULL, kSoundPlaybackNormal, NULL);
				RumbleManager::startEffect(&PointRumbleEffectSpec);

				if(false && 0 < Disk::getPosition(eventFirer)->x)
				{
					this->leftScore++;
				}
				else
				{
					this->rightScore++;
				}

				PongManager::printScore(this);
			}

			if(NULL != this->leftPaddle && eventFirer == ListenerObject::safeCast(this->leftPaddle))
			{
				this->leftPaddle = NULL;
			}
			else if(NULL != this->rightPaddle && eventFirer == ListenerObject::safeCast(this->rightPaddle))
			{
				this->rightPaddle = NULL;
			}

			return false;
		}

		case kEventActorCreated:
		{
			if(__GET_CAST(Paddle, eventFirer))
            {
				if(0 == strcmp(PADDLE_LEFT_NAME, Actor::getName(eventFirer)))
				{
					this->leftPaddle = Paddle::safeCast(eventFirer);
					Paddle::addEventListener(this->leftPaddle, ListenerObject::safeCast(this), kEventActorDeleted);
				}
				else if(0 == strcmp(PADDLE_RIGHT_NAME, Actor::getName(eventFirer)))
				{
					this->rightPaddle = Paddle::safeCast(eventFirer);
					Paddle::addEventListener(this->rightPaddle, ListenerObject::safeCast(this), kEventActorDeleted);
				}
			}

			return true;
		}
	}

	return Base::onEvent(this, eventFirer, eventCode);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::getReady(Stage stage)
{
	if(!isDeleted(stage))
	{
		this->leftPaddle = Paddle::safeCast(Stage::getChildByName(stage, (char*)PADDLE_LEFT_NAME, true));
		this->rightPaddle = Paddle::safeCast(Stage::getChildByName(stage, (char*)PADDLE_RIGHT_NAME, true));

		if(!isDeleted(this->leftPaddle))
		{
			Paddle::addEventListener(this->leftPaddle, ListenerObject::safeCast(this), kEventActorDeleted);
		}

		if(!isDeleted(this->rightPaddle))
		{
			Paddle::addEventListener(this->rightPaddle, ListenerObject::safeCast(this), kEventActorDeleted);
		}

		Stage::addActorLoadingListener(stage, ListenerObject::safeCast(this));
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::processUserInput(const UserInput* userInput)
{
	if(0 != userInput->holdKey)
	{
		PongManager::onKeyHold(this, userInput->holdKey);
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::onKeyHold(uint16 holdKey)
{
	NormalizedDirection normalizedDirection = {0, 0, 0};

	if((K_LU | K_RU) & holdKey)
	{
		normalizedDirection.y = __UP;
	}
	else if((K_LD | K_RD) & holdKey)
	{
		normalizedDirection.y = __DOWN;
	}

	if(NULL != this->leftPaddle)
	{
		Paddle::moveTowards(this->leftPaddle, normalizedDirection);
	}

	if(NULL != this->rightPaddle)
	{
		Paddle::moveTowards(this->rightPaddle, normalizedDirection);
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::printScore()
{
	Printer::int32(this->leftScore, 24 - 3, 0, NULL);
	Printer::int32(this->rightScore, 24 + 3, 0, NULL);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
