#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "BaseMenuInputHandler.h"

namespace SnakeGame
{
	class Game;
	class RecordsState;
	struct Settings; 
	enum class RecordStateWindowType;
	class RecordsStateNameMenu;
	class RecordsStateMenu;

	class RecordsStateNameDialogInputHandler : public BaseMenuInputHandler
	{
	public:
		RecordsStateNameDialogInputHandler() = delete;
		RecordsStateNameDialogInputHandler(RecordsStateNameMenu* currentMenu, RecordsState* currentState);
		virtual ~RecordsStateNameDialogInputHandler() = default;
	private:
		RecordsState* state;
		void ToNameTyping();
		void ToRecordTable();
	};

	class RecordsStateTableDialogInputHandler : public BaseMenuInputHandler
	{
	public:
		RecordsStateTableDialogInputHandler() = delete;
		RecordsStateTableDialogInputHandler(RecordsStateMenu* currentMenu, RecordsState* currentState);
		virtual ~RecordsStateTableDialogInputHandler() = default;
		void RestartGame();
		void ToMainMenu();
	};

	class RecordsStateNameEnteringInputHandler : public BaseInputHandler
	{
	public:
		RecordsStateNameEnteringInputHandler() = delete;
		RecordsStateNameEnteringInputHandler(RecordsState* currentState, sf::Text* nameText);
		virtual ~RecordsStateNameEnteringInputHandler() = default;
		virtual void HandleInputEvents(const std::vector<sf::Event>& input) override;
		void ToRecordTable();
		void RemoveSymbol();
	private:
		RecordsState* state;
		sf::Text* name;
	};

	class RecordsStateMenuInputHandler : public BaseInputHandler
	{
	public:
		RecordsStateMenuInputHandler();
		virtual ~RecordsStateMenuInputHandler() = default;
		void ToMainMenu();
	};
}

