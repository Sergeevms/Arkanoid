#include "RecordsState.h"
#include "RecordTable.h"
#include "RecordsStateInputHandlers.h"
#include "RecordsStateMenus.h"
#include "Settings.h"
#include "Game.h"

namespace Arkanoid
{
	RecordsState::RecordsState(const bool fromGame) : BaseState()
	{
		table = std::make_unique<RecordTable>();
		table->Deserialize();
		if (fromGame)
		{
			windows.emplace_back(std::make_unique<RecordStateGameWindow>(table.get(), this));
			windows.emplace_back(std::make_unique<RecordsStateEnterNameDialog>(this));
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
			windows.emplace_back(std::make_unique<NameEnteringWindow>(this));
			break;
		}
		case RecordStateWindowType::RecordTable:
		{
			NameEnteringWindow* window = dynamic_cast<NameEnteringWindow*>(windows.back().get());
			if (window)
			{
				table.get()->AddRecord(window->GetName(), Game::GetGame()->GetLastSessionScore());
			}
			else
			{
				table.get()->AddRecord(Settings::GetSettings()->defaultPlayerName, Game::GetGame()->GetLastSessionScore());
			}
			table.get()->Serialize();
			windows.pop_back();
			break;
		}			
		default:
			break;
		}
	}

	RecordsStateEnterNameDialog::RecordsStateEnterNameDialog(RecordsState* currentState) : menu(std::make_unique<RecordsStateNameMenu>())
	{
		inputHandler = std::make_unique<RecordsStateNameDialogInputHandler>(menu.get(), currentState);
		background.setFillColor({ 0, 0, 0, 255 });
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(Settings::GetSettings()->popUpBorder);
	}

	void RecordsStateEnterNameDialog::Draw(sf::RenderWindow& window)
	{
		sf::FloatRect rect = menu.get()->GetRect();
		background.setSize({ rect.width + Settings::GetSettings()->popUpSpacing, rect.height + Settings::GetSettings()->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(Settings::GetSettings()->ScreenCenter());
		window.draw(background);
		menu.get()->Draw(window, Settings::GetSettings()->ScreenCenter(), RelativePosition::Center);
	}

	NameEnteringWindow::NameEnteringWindow(RecordsState* state)
	{
		inputHandler = std::make_unique<RecordsStateNameEnteringInputHandler>(state, &name);
		textStyle.Init("Roboto-Regular.ttf");
		header.SetStyle(&textStyle);
		header.setString(L"Имя:");
		name.SetStyle(&textStyle);
		background.setFillColor({ 0, 0, 0, 255 });
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(Settings::GetSettings()->popUpBorder);
	}

	void NameEnteringWindow::Draw(sf::RenderWindow& window)
	{
		std::vector<IListDrawable*> itemList;
		itemList.push_back(&header);
		itemList.push_back(&name);

		Settings* settings = Settings::GetSettings();

		sf::FloatRect rect = GetListRect(itemList, { 0.f, 0.f }, RelativePosition::TopLeft, Orientation::Vertical, Alignment::Middle, 20.f);
		background.setSize({ rect.width + settings->popUpSpacing, rect.height + settings->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(settings->ScreenCenter());
		window.draw(background);
		DrawList(window, itemList, settings->ScreenCenter(), RelativePosition::Center, Orientation::Vertical,
			Alignment::Middle, 10.f);
	}

	std::wstring NameEnteringWindow::GetName() const
	{
		return name.getString().toWideString();
	}

	RecordStateMenuWindow::RecordStateMenuWindow(RecordTable* records) : table(records)
	{
		Settings* settings = Settings::GetSettings();
		inputHandler = std::make_unique<RecordsStateMenuInputHandler>();
		headerTextStyle.Init("Roboto-Regular.ttf", sf::Color::White, sf::Text::Bold, 40);	
		header.SetStyle(&headerTextStyle);
		header.setString(L"Таблица рекордов");

		rowTextStyle.Init("Roboto-Regular.ttf");
		textRows.reserve(settings->bigRecordsSize);
		for (int i = 0; i < settings->bigRecordsSize; ++i)
		{
			textRows.push_back(ListDrawableText());
			textRows.back().SetStyle(&rowTextStyle);
		}

		background.setFillColor(sf::Color::Black);
		background.setSize(settings->ScreenSize());
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		background.setPosition(settings->ScreenCenter());
	}

	void RecordStateMenuWindow::Draw(sf::RenderWindow& window)
	{
		window.draw(background);
		DrawableList rows(Orientation::Vertical, Alignment::Middle, 10.f);
		int rowNumber = 0;
		for (auto& rowText : table->GetRecords(Settings::GetSettings()->bigRecordsSize))
		{
			textRows[rowNumber].setString(rowText);
			rows.AddItem(&(textRows[rowNumber]));
			++rowNumber;
		}

		std::vector<IListDrawable*> list;
		list.push_back(&header);
		list.push_back(&rows);

		DrawList(window, list, Settings::GetSettings()->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
	}

	RecordStateGameWindow::RecordStateGameWindow(RecordTable* records, RecordsState* state) : table(records)
	{
		menu = std::make_unique<RecordsStateMenu>();

		Settings* settings = Settings::GetSettings();

		inputHandler = std::make_unique<RecordsStateTableDialogInputHandler>(menu.get(), state);
		headerTextStyle.Init("Roboto-Regular.ttf", sf::Color::White, sf::Text::Bold, 40);
		headerTexts.reserve(3);
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, L"Количество");
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, L"очков:");
		CreateListDrawableTextInVector(headerTexts, &headerTextStyle, std::to_wstring(Game::GetGame()->GetLastSessionScore()));

		rowTextStyle.Init("Roboto-Regular.ttf");
		textRows.reserve(settings->smallRecordsSize);
		for (int i = 0; i < settings->smallRecordsSize; ++i)
		{
			CreateListDrawableTextInVector(textRows, &rowTextStyle);
		}

		background.setFillColor({ 0, 0, 0, 255 });
		background.setOutlineColor(sf::Color::White);
		background.setOutlineThickness(settings->popUpBorder);
		background.setPosition(settings->ScreenCenter());

		overAllBackground.setFillColor({ 255, 255, 255, 128 });
		overAllBackground.setSize(settings->ScreenSize());
		overAllBackground.setPosition(settings->ScreenCenter());
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
		Settings* settings = Settings::GetSettings();
		for (auto& rowText : table->GetRecords(settings->smallRecordsSize))
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

		sf::FloatRect listRect = GetListRect(list, settings->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
		background.setSize({ listRect.width + settings->popUpSpacing, listRect.height + settings->popUpSpacing });
		SetOriginByRelative(background, relativePositions.at(RelativePosition::Center));
		window.draw(background);

		DrawList(window, list, settings->ScreenCenter(), RelativePosition::Center, Orientation::Vertical, Alignment::Middle, 40.f);
	}
}