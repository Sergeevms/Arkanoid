#include "PauseStateInputHandler.h"
#include "PauseMenu.h"
#include "Game.h"
#include "Settings.h"
#include "BaseState.h"

namespace SnakeGame
{
	PauseStateInputHandler::PauseStateInputHandler(PauseMenu* currentMenu) :
		BaseMenuInputHandler(currentMenu)
	{	actionMapping[ActionsTypesOnInput::Back] = [this](BaseInputHandler* handler)
		{if (auto currentHandler = dynamic_cast<PauseStateInputHandler*>(this)) { currentHandler->ReturnToGame(); }};

		activateMapping[MenuNodeActivateReaction::MainMenu] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PauseStateInputHandler*>(this)) { currentHandler->ReturnToMenu(); }};
		activateMapping[MenuNodeActivateReaction::Play] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PauseStateInputHandler*>(this)) { currentHandler->ReturnToGame(); }};
	}

	void PauseStateInputHandler::ReturnToGame()
	{
		Game::GetGame()->SwitchToState(GameState::Playing);
	}

	void PauseStateInputHandler::ReturnToMenu()
	{
		Game::GetGame()->SwitchToState(GameState::MainMenu);
	}
}