#include "PlayingInputHandler.h"
#include "BaseState.h"
#include "Application.h"

namespace Arkanoid
{
	PlayingInputHandler::PlayingInputHandler() :
		BaseInputHandler()
	{
		actionMapping[ActionsTypesOnInput::Pause] = 
			[](BaseInputHandler* handler)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Pause);
			};

		actionMapping[ActionsTypesOnInput::RecordTable] =
			[](BaseInputHandler* handler)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Records);
			};
	};
}