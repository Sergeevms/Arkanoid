#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "BaseInputHandler.h"

namespace Arkanoid
{
	class Game;
	class PlayingState;
	struct Settings;

	class PlayingInputHandler : public BaseInputHandler
	{
	public:
		PlayingInputHandler(PlayingState* currentState);
		void PauseGame();
	private:
		PlayingState* state;
	};
}