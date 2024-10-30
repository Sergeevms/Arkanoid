#include "PauseState.h"
#include "Application.h"
#include "Utility.h"
#include "Settings.h"
#include "BaseMenuInputHandler.h"

namespace Arkanoid
{
	PauseState::PauseState() : 
		BaseState(), menu()
	{
		inputHandler = std::make_unique<BaseMenuInputHandler>(&menu);
		Settings* settings = Application::GetSettings();
		overallBackground.setPosition(relativePositions.at(RelativePosition::TopLeft));
		overallBackground.setSize(settings->ScreenSize());
		overallBackground.setFillColor(settings->halfTrasparentWhite);

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
		menu.Draw(window, { Application::GetSettings()->ScreenCenter() }, RelativePosition::Center);
	}
}