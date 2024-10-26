#include <functional>
#include "PlayingInputHandler.h"
#include "PlayingState.h"
#include "Game.h"
#include "Snake.h"
#include "Settings.h"

namespace SnakeGame
{
	PlayingInputHandler::PlayingInputHandler(Snake* currentSnake, PlayingState* currentState)
		: BaseInputHandler(), snake(currentSnake), state(currentState)
	{
		actionMapping[ActionsTypesOnInput::Up] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->TurnSnakeUp(); }};		
		actionMapping[ActionsTypesOnInput::Down] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->TurnSnakeDown(); }}; 
		actionMapping[ActionsTypesOnInput::Left] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->TurnSnakeLeft(); }};
		actionMapping[ActionsTypesOnInput::Right] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->TurnSnakeRight(); }};
		actionMapping[ActionsTypesOnInput::Pause] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<PlayingInputHandler*>(this)) { currentHandler->PauseGame(); }}; 
	};

	void PlayingInputHandler::TurnSnakeUp()
	{
		snake->SetNewDirection(Direction::Up);
	}

	void PlayingInputHandler::TurnSnakeRight()
	{
		snake->SetNewDirection(Direction::Rigth);
	}

	void PlayingInputHandler::TurnSnakeDown()
	{
		snake->SetNewDirection(Direction::Down);
	}

	void PlayingInputHandler::TurnSnakeLeft()
	{
		snake->SetNewDirection(Direction::Left);
	}

	void PlayingInputHandler::PauseGame()
	{
		state->resetMovingDelay();
		Game::GetGame()->SwitchToState(GameState::Pause);
	}
}