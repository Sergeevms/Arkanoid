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
		
		currentNode = InitializeRootNode(L"Пауза", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"Выйти из игры", &selectedStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->ToMainMenu();
			});

		InitializeNode(currentNode, L"Сохранить игру и выйти", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SaveGameAndGoToMenu();
			});

		InitializeNode(currentNode, L"Продолжить", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->StartGame();
			});
	}
}