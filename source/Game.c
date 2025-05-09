/*
 * Pong
 *
 * © VUEngine Studio User
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <AdjustmentScreenState.h>
#include <AutomaticPauseManager.h>
#include <AutomaticPauseSelectionScreenState.h>
#include <GameSaveDataManager.h>
#include <I18n.h>
#include <LanguageSelectionScreenState.h>
#include <TitleScreenState.h>
#include <PrecautionScreenState.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// GAME'S ENTRY POINT
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

GameState game(void)
{
	// Initialize plugins
	AutomaticPauseManager::setActive
	(
		AutomaticPauseManager::getInstance(),
		GameSaveDataManager::getAutomaticPauseStatus(GameSaveDataManager::getInstance())
	);
	I18n::setActiveLanguage(I18n::getInstance(), GameSaveDataManager::getLanguage(GameSaveDataManager::getInstance()));

	SplashScreenState::setNextState
	(
		SplashScreenState::safeCast(PrecautionScreenState::getInstance()),
		GameState::safeCast(AdjustmentScreenState::getInstance())
	);

	SplashScreenState::setNextState
	(
		SplashScreenState::safeCast(AdjustmentScreenState::getInstance()),
		GameState::safeCast(AutomaticPauseSelectionScreenState::getInstance())
	);

	SplashScreenState::setNextState
	(
		SplashScreenState::safeCast(AutomaticPauseSelectionScreenState::getInstance()),
		GameState::safeCast(LanguageSelectionScreenState::getInstance())
	);

	SplashScreenState::setNextState
	(
		SplashScreenState::safeCast(LanguageSelectionScreenState::getInstance()),
		GameState::safeCast(TitleScreenState::getInstance())
	);

	// Return the first GameState
	return GameState::safeCast(PrecautionScreenState::getInstance());
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
