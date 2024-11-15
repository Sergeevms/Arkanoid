#include "GameWinnedState.h"
#include "GameWinnedMenu.h"
#include "BaseMenuInputHandler.h"
#include "Application.h"

namespace Arkanoid
{
	GameWinnedState::GameWinnedState()
	{
		gameOverMenu = std::make_unique<GameWinnedMenu>();
		inputHandler = std::make_unique<BaseMenuInputHandler>(gameOverMenu.get());

		background.setFillColor(GameWorld::GetWorld()->halfTrasparentWhite);
		background.setSize(GameWorld::GetWorld()->ScreenSize());
	}

	void GameWinnedState::Draw(sf::RenderWindow& window) const
	{
		window.draw(background);
		gameOverMenu->Draw(window, GameWorld::GetWorld()->ScreenCenter(), RelativePosition::Center);
	}
}