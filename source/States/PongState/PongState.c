/*
 * Pong
 *
 * © VUEngine Studio User
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <string.h>

#include <Camera.h>
#include <CameraEffectManager.h>
#include <I18n.h>
#include <Languages.h>
#include <Paddle.h>
#include <Singleton.h>
#include <VIPManager.h>
#include <VUEngine.h>

#include "PongState.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern StageROMSpec PongStageSpec;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PUBLIC METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::enter(void* owner __attribute__((unused)))
{
	Base::enter(this, owner);

	// Load stage
	PongState::configureStage(this, (StageSpec*)&PongStageSpec, NULL);

	// Start clocks to start animations
	PongState::startClocks(this);

	// Enable user input
	KeypadManager::enable();

	// Start fade in effect
	Camera::startEffect(Camera::getInstance(), kHide);
	
	Camera::startEffect
	(
		Camera::getInstance(),
		kFadeTo,	   // effect type
		0,			   // initial delay (in ms)
		NULL,		   // target brightness
		__FADE_DELAY,  // delay between fading steps (in ms)
		NULL		   // callback scope
	);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::execute(void* owner __attribute__((unused)))
{
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::exit(void* owner __attribute__((unused)))
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

    Base::exit(this, owner);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::resume(void* owner)
{
	Base::resume(this, owner);

	// Enable user input
	KeypadManager::enable();

	Camera::startEffect(Camera::getInstance(), kHide);

	Camera::startEffect
	(
		Camera::getInstance(),
		kFadeTo,	   // effect type
		0,			   // initial delay (in ms)
		NULL,		   // target brightness
		__FADE_DELAY,  // delay between fading steps (in ms)
		NULL		   // callback scope
	);
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::processUserInput(const UserInput* userInput)
{
	Paddle paddle = Stage::getChildByName(this->stage, "PaddleL", true);

	if(NULL != paddle)
	{

		NormalizedDirection normalizedDirection = {0, 0, 0};

		if((K_LU | K_RU) & userInput->holdKey)
		{
			normalizedDirection.y = __UP;
			Paddle::moveTowards(paddle, normalizedDirection);
		}
		else if((K_LD | K_RD) & userInput->holdKey)
		{
			normalizedDirection.y = __DOWN;
			Paddle::moveTowards(paddle, normalizedDirection);
		}
	}
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' PRIVATE METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::constructor()
{
	// Always explicitly call the base's constructor
	Base::constructor();
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void PongState::destructor()
{
	// Always explicitly call the base's destructor
	Base::destructor();
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
