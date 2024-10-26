#include "RecordsStateInputHandlers.h"
#include "Settings.h"
#include "Game.h"
#include "RecordsStateMenus.h"
#include "RecordsState.h"

namespace SnakeGame
{
    RecordsStateNameDialogInputHandler::RecordsStateNameDialogInputHandler(RecordsStateNameMenu* currentMenu, RecordsState* currentState) :
		BaseMenuInputHandler(currentMenu), state(currentState)
    {
		activateMapping[MenuNodeActivateReaction::EnterName] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateNameDialogInputHandler*>(this)) { currentHandler->ToNameTyping(); }};
		activateMapping[MenuNodeActivateReaction::SkipName] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateNameDialogInputHandler*>(this)) { currentHandler->ToRecordTable(); }};
    }

	void RecordsStateNameDialogInputHandler::ToNameTyping()
	{
		state->SwitchToWindow(RecordStateWindowType::NameInput);
	}

	void RecordsStateNameDialogInputHandler::ToRecordTable()
	{
		state->SwitchToWindow(RecordStateWindowType::RecordTable);
	}

	RecordsStateTableDialogInputHandler::RecordsStateTableDialogInputHandler(RecordsStateMenu* currentMenu, RecordsState* currentState) :
		BaseMenuInputHandler(currentMenu)
	{
		activateMapping[MenuNodeActivateReaction::MainMenu] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateTableDialogInputHandler*>(this)) { currentHandler->ToMainMenu(); }};
		activateMapping[MenuNodeActivateReaction::Play] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateTableDialogInputHandler*>(this)) { currentHandler->RestartGame(); }};
	}

	void RecordsStateTableDialogInputHandler::RestartGame()
	{
		Game::GetGame()->SwitchToState(GameState::Playing);
	}

	void RecordsStateTableDialogInputHandler::ToMainMenu()
	{
		Game::GetGame()->SwitchToState(GameState::MainMenu);
	}

	RecordsStateNameEnteringInputHandler::RecordsStateNameEnteringInputHandler(RecordsState* currentState, sf::Text* nameText) :
		BaseInputHandler(), state(currentState), name(nameText)
	{
		actionMapping[ActionsTypesOnInput::BackSpace] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateNameEnteringInputHandler*>(this)) { currentHandler->RemoveSymbol(); }};
		actionMapping[ActionsTypesOnInput::Forward] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateNameEnteringInputHandler*>(this)) { currentHandler->ToRecordTable(); }};
	}

	void RecordsStateNameEnteringInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{
		for (auto& inputEvent : input)
		{
			switch (inputEvent.type)
			{
			case (sf::Event::TextEntered):
			{
				if (inputEvent.text.unicode >= 0x20)
				{			
					name->setString(name->getString() + sf::String(inputEvent.text.unicode));
				}
				break;
			}
			case (sf::Event::KeyPressed):
			{
				Settings* settings = Settings::GetSettings();
				if (settings->keyMap.contains(inputEvent.key.code))
				{
					if (actionMapping.contains(settings->keyMap[inputEvent.key.code]))
					{
						Game::GetGame()->PlaySound(SoundType::OnKeyHit);
						actionMapping.at(settings->keyMap[inputEvent.key.code]) (this);
					}
				}
				break;
			}
			}
		}
	}

	void RecordsStateNameEnteringInputHandler::ToRecordTable()
	{
		state->SwitchToWindow(RecordStateWindowType::RecordTable);
	}

	void RecordsStateNameEnteringInputHandler::RemoveSymbol()
	{		
		sf::String newName = name->getString();
		if (newName.getSize() > 0)
		{
			newName.erase(newName.getSize() - 1, 1);
		}
		name->setString(newName);
	}

	RecordsStateMenuInputHandler::RecordsStateMenuInputHandler()
	{
		actionMapping[ActionsTypesOnInput::Back] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<RecordsStateMenuInputHandler*>(this)) { currentHandler->ToMainMenu(); }};
	}

	void RecordsStateMenuInputHandler::ToMainMenu()
	{
		Game::GetGame()->SwitchToState(GameState::MainMenu);
	}
}
