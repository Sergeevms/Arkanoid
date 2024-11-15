#include "RecordsState.h"
#include "RecordTable.h"
#include "Application.h"
#include "RecordsStateInputHandlers.h"
#include "RecordsStateMenus.h"
#include "GameWorld.h"
#include "Game.h"

namespace Arkanoid
{
	RecordsState::RecordsState(const bool fromGame) : BaseState()
	{
		table = std::make_unique<RecordTable>();
		table->Deserialize();
		if (fromGame)
		{
			windows.emplace_back(std::make_unique<RecordStateGameWindow>(table.get()));
			windows.emplace_back(std::make_unique<RecordsStateEnterNameDialog>());
		}
		else
		{
			windows.emplace_back(std::make_unique<RecordStateMenuWindow>(table.get()));
		}
	}

	void RecordsState::Draw(sf::RenderWindow& window) const
	{
		for (auto& curWindow : windows)
		{
			curWindow->Draw(window);
		}
	}

	void RecordsState::HandleInput(std::vector<sf::Event> const& input)
	{
		windows.back()->HandleInput(input);
	}

	void RecordsState::SwitchToWindow(RecordStateWindowType windowType)
	{
		switch (windowType)
		{
		case RecordStateWindowType::NameInput:
		{
			windows.pop_back();
			windows.emplace_back(std::make_unique<NameEnteringWindow>());
			break;
		}
		case RecordStateWindowType::RecordTable:
		{
			NameEnteringWindow* window = dynamic_cast<NameEnteringWindow*>(windows.back().get());
			if (window)
			{
				table.get()->AddRecord(window->GetName(), Application::GetInstance().GetGame()->GetLastSessionScore());
			}
			else
			{
				table.get()->AddRecord(GameWorld::GetWorld()->defaultPlayerName, Application::GetInstance().GetGame()->GetLastSessionScore());
			}
			table.get()->Serialize();
			windows.pop_back();
			break;
		}			
		default:
			break;
		}
	}

	RecordsStateEnterNameDialog::RecordsStateEnterNameDialog() : menu(std::make_unique<RecordsStateNameMenu>())
	{
		inputHandler = std::make_unique<BaseMenuInputHandler>(menu.get());
		background.setFillColor(sf::Color::Black);
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(GameWorld::GetWorld()->popUpBorder);
	}

	void RecordsStateEnterNameDialog::Draw(sf::RenderWindow& window)
	{
		sf::FloatRect rect = menu.get()->GetRect();
		background.setSize({ rect.width + GameWorld::GetWorld()->popUpSpacing, rect.height + GameWorld::GetWorld()->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(GameWorld::GetWorld()->ScreenCenter());
		window.draw(background);
		menu.get()->Draw(window, GameWorld::GetWorld()->ScreenCenter(), RelativePosition::Center);
	}

	NameEnteringWindow::NameEnteringWindow()
	{
		inputHandler = std::make_unique<RecordsStateNameEnteringInputHandler>(&name);
		textStyle.Init("Roboto-Regular.ttf");
		header.SetStyle(&textStyle);
		header.setString(L"Имя:");
		name.SetStyle(&textStyle);
		background.setFillColor(sf::Color::Black);
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(GameWorld::GetWorld()->popUpBorder);
	}

	void NameEnteringWindow::Draw(sf::RenderWindow& window)
	{
		std::vector<IListDrawable*> itemList;
		itemList.push_back(&header);
		itemList.push_back(&name);

		GameWorld* world = GameWorld::GetWorld();

		sf::FloatRect rect = GetListRect(itemList, { 0.f, 0.f }, RelativePosition::TopLeft, Orientation::Vertical, Alignment::Middle, 20.f);
		background.setSize({ rect.width + world->popUpSpacing, rect.height + world->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(world->ScreenCenter());
		window.draw(background);
		DrawList(window, itemList, world->ScreenCenter(), RelativePosition::Center, Orientation::Vertical,
			Alignment::Middle, 10.f);
	}

	std::wstring NameEnteringWindow::GetName() const
	{
		return name.getString().toWideString();
	}

	RecordStateMenuWindow::RecordStateMenuWindow(RecordTable* records) : table(records)
	{
		GameWorld* world = GameWorld::GetWorld();
		inputHandler = std::make_unique<RecordsStateMenuInputHandler>();
		headerTextStyle.Init("Roboto-Regular.ttf", sf::Color::White, sf::Text::Bold, 40);	
		header.SetStyle(&headerTextStyle);
		header.setString(L"Таблица рекордов");

		rowTextStyle.Init("Roboto-Regular.ttf");
		textRows.reserve(world->bigRecordsSize);
		for (int i = 0; i < world->bigRecordsSize; ++i)
		{
			textRows.push_back(ListDrawableText());
			textRows.back().SetStyle(&rowTextStyle);
		}

		background.setFillColor(sf::Color::Black);
		background.setSize(world->ScreenSize());
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(world->ScreenCenter());
	}

	void RecordStateMenuWindow::Draw(sf::RenderWindow& window)
	{
		window.draw(background);
		DrawableList rows(Orientation::Vertical, Alignment::Middle, 10.f);
		int rowNumber = 0;
		for (auto& rowText : table->GetRecords(GameWorld::GetWorld()->bigRecordsSize))
		{
			textRows[rowNumber].setString(rowText);
			rows.AddItem(&(textRows[rowNumber]));
			++rowNumber;
		}

		std::vector<IListDrawable*> list;
		list.push_back(&header);
		list.push_back(&rows);

		DrawList(window, list, GameWorld::GetWorld()->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
	}

	RecordStateGameWindow::RecordStateGameWindow(RecordTable* records) : table(records)
	{
		menu = std::make_unique<RecordsStateMenu>();

		GameWorld* world = GameWorld::GetWorld();

		inputHandler = std::make_unique<BaseMenuInputHandler>(menu.get());
		headerTextStyle.Init("Roboto-Regular.ttf", sf::Color::White, sf::Text::Bold, 40);
		headerTexts.reserve(3);
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, L"Количество");
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, L"очков:");
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, std::to_wstring(Application::GetInstance().GetGame()->GetLastSessionScore()));

		rowTextStyle.Init("Roboto-Regular.ttf");
		textRows.reserve(world->smallRecordsSize);
		for (int i = 0; i < world->smallRecordsSize; ++i)
		{
			CreateListDrawableTextInVector(textRows, &rowTextStyle);
		}

		background.setFillColor(sf::Color::Black);
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(world->popUpBorder);
		background.setPosition(world->ScreenCenter());

		overAllBackground.setFillColor(world->halfTrasparentWhite);
		overAllBackground.setSize(world->ScreenSize());
		overAllBackground.setPosition(world->ScreenCenter());
		SetOriginByRelative(overAllBackground, relativePositions.at(RelativePosition::Center));
	}

	void RecordStateGameWindow::Draw(sf::RenderWindow& window)
	{
		DrawableList header(Orientation::Vertical, Alignment::Middle, 5.f);
		for (auto& row : headerTexts)
		{
			header.AddItem(&row);
		}

		DrawableList rows(Orientation::Vertical, Alignment::Middle, 10.f);
		int rowNumber = 0;
		GameWorld* world = GameWorld::GetWorld();
		for (auto& rowText : table->GetRecords(world->smallRecordsSize))
		{
			textRows[rowNumber].setString(rowText);
			rows.AddItem(&(textRows[rowNumber]));
			++rowNumber;
		}

		std::vector<IListDrawable*> list;
		list.push_back(&header);
		list.push_back(&rows);
		list.push_back(menu.get());

		window.draw(overAllBackground);

		sf::FloatRect listRect = GetListRect(list, world->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
		background.setSize({ listRect.width + world->popUpSpacing, listRect.height + world->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		window.draw(background);

		DrawList(window, list, world->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
	}
}