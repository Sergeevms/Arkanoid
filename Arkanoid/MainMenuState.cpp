#include "MainMenuState.h"
#include "Settings.h"
#include "MainMenuInputHandler.h"

namespace SnakeGame
{
	MainMenuState::MainMenuState() 
	{
		inputHandler = std::make_unique<MainMenuInputHandler>(&menu);
	}

	void MainMenuState::Draw(sf::RenderWindow& window) const
	{
		menu.Draw(window, { Settings::GetSettings()->ScreenCenter() }, RelativePosition::Center);
	}
}