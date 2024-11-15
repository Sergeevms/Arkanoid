#include "PauseState.h"
#include "Application.h"
#include "Utility.h"
#include "GameWorld.h"
#include "BaseMenuInputHandler.h"

namespace Arkanoid
{
	PauseState::PauseState() : 
		BaseState(), menu()
	{
		inputHandler = std::make_unique<BaseMenuInputHandler>(&menu);
		GameWorld* world = GameWorld::GetWorld();
		overallBackground.setPosition(relativePositions.at(RelativePosition::TopLeft));
		overallBackground.setSize(world->ScreenSize());
		overallBackground.setFillColor(world->halfTrasparentWhite);

		sf::FloatRect menuRect = menu.GetRect();
		menuBackground.setSize({menuRect.width + world->popUpSpacing, menuRect.height + world->popUpSpacing });
		SetOriginByRelative(menuBackground, relativePositions.at(RelativePosition::Center));
		menuBackground.setPosition(world->ScreenCenter());
		menuBackground.setFillColor(sf::Color::Black);
		menuBackground.setOutlineColor(sf::Color::White);
		menuBackground.setOutlineThickness(-3.f);
	}

	void PauseState::Draw(sf::RenderWindow& window) const
	{
		window.draw(overallBackground);
		window.draw(menuBackground);
		menu.Draw(window, { GameWorld::GetWorld()->ScreenCenter() }, RelativePosition::Center);
	}
}