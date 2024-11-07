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
		currentNode = InitializeRootNode(L"������! ��� ���?", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"��", &selectedStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Playing);
			});
		InitializeNode(currentNode, L"���", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::MainMenu);
			});
	}
}