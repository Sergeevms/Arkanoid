#include "MainMenuState.h"
#include "Settings.h"
#include "BaseMenuInputHandler.h"
#include "Application.h"

namespace Arkanoid
{
	MainMenuState::MainMenuState() 
	{
		inputHandler = std::make_unique<BaseMenuInputHandler>(&menu);
	}

	void MainMenuState::Draw(sf::RenderWindow& window) const
	{
		menu.Draw(window, { Application::GetSettings()->ScreenCenter() }, RelativePosition::Center);
	}
}