#include "PauseState.h"
#include "Utility.h"
#include "Settings.h"
#include "PauseStateInputHandler.h"

namespace SnakeGame
{
	PauseState::PauseState() : 
		BaseState(), menu()
	{
		inputHandler = std::make_unique<PauseStateInputHandler>(&menu);
		Settings* settings = Settings::GetSettings();
		overallBackground.setPosition(relativePositions.at(RelativePosition::TopLeft));
		overallBackground.setSize({ static_cast<float>(settings->screenWidth), static_cast<float>(settings->screenHeight) });
		overallBackground.setFillColor({255, 255, 255, 125});

		sf::FloatRect menuRect = menu.GetRect();
		menuBackground.setSize({menuRect.width + settings->popUpSpacing, menuRect.height + settings->popUpSpacing });
		SetOriginByRelative(menuBackground, relativePositions.at(RelativePosition::Center));
		menuBackground.setPosition(settings->ScreenCenter());
		menuBackground.setFillColor(sf::Color::Black);
		menuBackground.setOutlineColor(sf::Color::White);
		menuBackground.setOutlineThickness(-3.f);
	}

	void PauseState::Draw(sf::RenderWindow& window) const
	{
		window.draw(overallBackground);
		window.draw(menuBackground);
		menu.Draw(window, { Settings::GetSettings()->ScreenCenter() }, RelativePosition::Center);
	}
}