#include "GameWinnedMenu.h"
#include "Application.h"
#include "BaseState.h"

namespace Arkanoid
{
	GameWinnedMenu::GameWinnedMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf", sf::Color::Green, sf::Text::Style(sf::Text::Style::Bold), 60);
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		currentNode = InitializeRootNode(L"Победа! Ещё раз?", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"Да", &selectedStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Playing);
			});
		InitializeNode(currentNode, L"Нет", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::MainMenu);
			});
	}
}