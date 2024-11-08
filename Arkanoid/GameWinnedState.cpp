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

		background.setFillColor(Application::GetSettings()->halfTrasparentWhite);
		background.setSize(Application::GetSettings()->ScreenSize());
	}

	void GameWinnedState::Draw(sf::RenderWindow& window) const
	{
		window.draw(background);
		gameOverMenu->Draw(window, Application::GetSettings()->ScreenCenter(), RelativePosition::Center);
	}
}