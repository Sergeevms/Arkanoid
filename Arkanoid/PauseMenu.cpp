#include "PauseMenu.h"

namespace Arkanoid
{
	PauseMenu::PauseMenu() : GeneralMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf", sf::Color::Green, sf::Text::Style(sf::Text::Style::Bold), 50);
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		
		currentNode = InitializeRootNode(L"Пауза", &headerStyle, MenuNodeActivateReaction::None, &subMenuStyle);
		InitializeNode(currentNode, L"Выйти из игры", &selectedStyle, MenuNodeActivateReaction::MainMenu);
		InitializeNode(currentNode, L"Продолжить", &normalStyle, MenuNodeActivateReaction::Play);
	}
}