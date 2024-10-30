#include "RecordsStateMenus.h"
#include "Application.h"
#include "RecordsState.h"

namespace Arkanoid
{
	RecordsStateNameMenu::RecordsStateNameMenu() : GeneralMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf", sf::Color::White, sf::Text::Style(sf::Text::Style::Bold));
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
				
		currentNode = InitializeRootNode(L"Ввести имя?", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"Нет", &selectedStyle,
			[](MenuNode*)
			{
				if (auto state = dynamic_cast<RecordsState*>(Application::GetInstance().GetGame()->GetState()))
				{
					state->SwitchToWindow(RecordStateWindowType::RecordTable);
				}
			});
		InitializeNode(currentNode, L"Да", &normalStyle,
			[](MenuNode*)
			{
				if (auto state = dynamic_cast<RecordsState*>(Application::GetInstance().GetGame()->GetState()))
				{
					state->SwitchToWindow(RecordStateWindowType::NameInput);
				}
			});
	}

	RecordsStateMenu::RecordsStateMenu() : GeneralMenu()
	{
		headerStyle.Init("Roboto-Regular.ttf");
		normalStyle.Init("Roboto-Regular.ttf");
		selectedStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		
		currentNode = InitializeRootNode(L"", &headerStyle, &subMenuStyle);
		InitializeNode(currentNode, L"Начать игру", &selectedStyle, 
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Playing);
			});
		InitializeNode(currentNode, L"В главное меню", &normalStyle,
			[](MenuNode*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::MainMenu);
			});
	}

	void RecordsStateMenu::Draw(sf::RenderWindow& window, const sf::Vector2f& position, RelativePosition origin) const
	{
		MenuStyle* style = currentNode->GetMenuStyle();

		std::vector<IListDrawable*> itemList;

		std::vector<MenuNode*> childNodes = currentNode->GetChilds();
		for (auto& currentChild : childNodes)
		{
			itemList.push_back(currentChild);
		}		

		sf::FloatRect rect = GetListRect(itemList, position, RelativePositionByOrientationAndAlignment(style->orientation, style->alignment),
			style->orientation, style->alignment, style->spacing * 3.f);
		sf::Vector2f correctedPosition;
		correctedPosition.x = position.x - rect.width * (relativePositions.at(origin).x -
			relativePositions.at(RelativePositionByOrientationAndAlignment(style->orientation, style->alignment)).x);
		correctedPosition.y = position.y - rect.height * (relativePositions.at(origin).y -
			relativePositions.at(RelativePositionByOrientationAndAlignment(style->orientation, style->alignment)).y);

		DrawList(window, itemList, correctedPosition,
			RelativePositionByOrientationAndAlignment(style->orientation, style->alignment), style->orientation, style->alignment, style->spacing);
	}
}