#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Settings.h"

namespace SnakeGame
{
	class MainLoop
	{
	public:
		MainLoop();
		~MainLoop() = default;
		void Run();
	private:
		std::unique_ptr<sf::RenderWindow> window;
	};

}
