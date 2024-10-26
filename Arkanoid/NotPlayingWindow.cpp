#include "NotPlayingWindow.h"
#include "BaseInputHandler.h"

namespace SnakeGame
{
	void NotPlayingWindow::HandleInput(const std::vector<sf::Event>& input)
	{
		inputHandler->HandleInputEvents(input);
	}
}