#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "BaseInputHandler.h"

namespace SnakeGame
{
	class Game;
	class Snake;
	class PlayingState;
	struct Settings;

	class PlayingInputHandler : public BaseInputHandler
	{
	public:
		PlayingInputHandler(Snake* currentSnake, PlayingState* currentState);
		void TurnSnakeUp();
		void TurnSnakeRight();
		void TurnSnakeDown();
		void TurnSnakeLeft();
		void PauseGame();
	private:
		Snake* snake;
		PlayingState* state;
	};
}