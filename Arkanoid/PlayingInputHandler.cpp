#include <functional>
#include "PlayingInputHandler.h"
#include "PlayingState.h"
#include "Game.h"
#include "Settings.h"

namespace Arkanoid
{
	PlayingInputHandler::PlayingInputHandler(PlayingState* currentState)
		: BaseInputHandler(), state(currentState)
	{

		actionMapping[ActionsTypesOnInput::Pause] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->PauseGame(); }}; 
	};

	void PlayingInputHandler::PauseGame()
	{
		Game::GetGame()->SwitchToState(GameState::Pause);
	}
}