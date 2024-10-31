#include "PlayingInputHandler.h"
#include "BaseState.h"
#include "Application.h"
#include "Platform.h"
#include "Utility.h"

namespace Arkanoid
{
	PlayingInputHandler::PlayingInputHandler(Platform* platform) : BaseInputHandler(), platform(platform)		
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
	}

	void PlayingInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{
		BaseInputHandler::HandleInputEvents(input);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			platform->SetMovingDirection(Direction::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			platform->SetMovingDirection(Direction::Rigth);
		}
	}
}