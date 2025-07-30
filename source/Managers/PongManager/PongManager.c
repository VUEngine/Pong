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

#include <CommunicationManager.h>
#include <GameEvents.h>
#include <KeypadManager.h>
#include <Messages.h>
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

	this->leftScore = 0;
	this->rightScore = 0;

	this->stage = stage;

	PongManager::printScore(this);

	if(!isDeleted(this->stage))
	{
		Actor disk = Actor::safeCast(Stage::getChildByName(this->stage, (char*)DISK_NAME, false));

		if(!isDeleted(disk))
		{
			Actor::addEventListener(disk, ListenerObject::safeCast(this), kEventActorDeleted);
		}

		Stage::addActorLoadingListener(this->stage, ListenerObject::safeCast(this));
	}

	Printer::addEventListener(Printer::getInstance(), ListenerObject::safeCast(this), kEventFontRewritten);

	CommunicationManager::enableCommunications(CommunicationManager::getInstance(), ListenerObject::safeCast(this));

	PongManager::sendMessageToSelf(this, kMessageStartGame, 1000, 0);

	// Disable the gameplay for a few cycles
	KeypadManager::disable();
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::destructor()
{
	// Always explicitly call the base's destructor
	Base::destructor();
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool PongManager::onEvent(ListenerObject eventFirer, uint16 eventCode)
{
	switch(eventCode)
	{
		case kEventCommunicationsConnected:
		{
			// First, discard any previous message to start the game
			PongManager::discardMessages(this, kMessageStartGame);

			// Then call the method that setups the game to start the versus match
			PongManager::startVersusMode(this);
			return false;
		}

		case kEventFontRewritten:
		{
			PongManager::printScore(this);

			return true;
		}

		case kEventActorDeleted:
		{
			if(0 == strcmp(DISK_NAME, Actor::getName(eventFirer)))
			{
				SoundManager::playSound(&PointSoundSpec,  NULL, kSoundPlaybackNormal, NULL);
				RumbleManager::startEffect(&PointRumbleEffectSpec);

				if(0 < Actor::getPosition(eventFirer)->x)
				{
					this->leftScore++;
				}
				else
				{
					this->rightScore++;
				}

				PongManager::printScore(this);
			}

			return false;
		}

		case kEventActorCreated:
		{
			if(0 == strcmp(DISK_NAME, Actor::getName(eventFirer)))
			{
				Actor::addEventListener(eventFirer, ListenerObject::safeCast(this), kEventActorDeleted);

				KeypadManager::disable();

				PongManager::sendMessageToSelf(this, kMessageStartGame, 100, 0);
			}

			return true;
		}
	}

	return Base::onEvent(this, eventFirer, eventCode);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool PongManager::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMessageStartGame:
		{
			if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
			{
                // Must make sure that both systems are in sync before starting the game
				CommunicationManager::startSyncCycle(CommunicationManager::getInstance());
			}

			// Propagate the message to start the game
			Stage::propagateMessage(this->stage, Container::onPropagatedMessage, kMessageStartGame);

			// Since we are using the method processUserInput to sync both system, 
			// we must make sure that it is called regardless of local input
			KeypadManager::enableDummyKey();
			KeypadManager::enable();

			break;
		}
	}

	return true;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::startVersusMode()
{
	// Reprint the score
	this->leftScore = 0;
	this->rightScore = 0;

	PongManager::printScore(this);

	// Propagate the message about the versus mode player assigned to the local system
	Stage::propagateMessage
	(
		this->stage, Container::onPropagatedMessage, 
		CommunicationManager::isMaster(CommunicationManager::getInstance()) ? kMessageVersusModePlayer1 : kMessageVersusModePlayer2
	);

	// Delay the start of the versus mode
	PongManager::sendMessageToSelf(this, kMessageStartGame, 250, 0);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongManager::printScore()
{
	Printer::int32(this->leftScore, 24 - 3, 0, NULL);
	Printer::int32(this->rightScore, 24 + 3, 0, NULL);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
