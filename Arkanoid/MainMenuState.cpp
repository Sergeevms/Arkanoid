#include "MainMenuState.h"
#include "GameWorld.h"
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
		menu.Draw(window, { GameWorld::GetWorld()->ScreenCenter() }, RelativePosition::Center);
	}
}