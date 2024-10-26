#include "BaseState.h"
#include "BaseInputHandler.h"

namespace SnakeGame
{
	void BaseState::HandleInput(const std::vector<sf::Event>& input)
	{
		inputHandler->HandleInputEvents(input);
	}
}