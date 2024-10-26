#include "NotPlayingWindow.h"
#include "BaseInputHandler.h"

namespace Arkanoid
{
	void NotPlayingWindow::HandleInput(const std::vector<sf::Event>& input)
	{
		inputHandler->HandleInputEvents(input);
	}
}