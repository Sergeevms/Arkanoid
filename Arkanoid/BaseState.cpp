#include "BaseState.h"
#include "BaseInputHandler.h"

namespace Arkanoid
{
	void BaseState::HandleInput(const std::vector<sf::Event>& input)
	{
		inputHandler->HandleInputEvents(input);
	}
}