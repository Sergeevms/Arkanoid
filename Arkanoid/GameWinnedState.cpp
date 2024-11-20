#include "GameWinnedState.h"
#include "GameWinnedMenu.h"
#include "BaseMenuInputHandler.h"
#include "Application.h"

namespace Arkanoid
{
	GameWinnedState::GameWinnedState()
	{
		gameWinnedMenu = std::make_unique<GameWinnedMenu>();
		inputHandler = std::make_unique<BaseMenuInputHandler>(gameWinnedMenu.get());

		background.setFillColor(GameWorld::GetWorld()->halfTrasparentWhite);
		background.setSize(GameWorld::GetWorld()->ScreenSize());
	}

	void GameWinnedState::Draw(sf::RenderWindow& window) const
	{
		window.draw(background);
		gameWinnedMenu->Draw(window, GameWorld::GetWorld()->ScreenCenter(), RelativePosition::Center);
	}
}