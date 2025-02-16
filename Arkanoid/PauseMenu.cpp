#include "PauseMenu.h"
#include "Application.h"
#include "BaseState.h"

namespace Arkanoid
{
	PauseMenu::PauseMenu() : GeneralMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf", sf::Color::Green, sf::Text::Style(sf::Text::Style::Bold), 50);
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		
		currentNode = InitializeRootNode(L"�����", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"����� �� ����", &selectedStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->ToMainMenu();
			});

		InitializeNode(currentNode, L"��������� ���� � �����", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SaveGameAndGoToMenu();
			});

		InitializeNode(currentNode, L"����������", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->StartGame();
			});
	}
}