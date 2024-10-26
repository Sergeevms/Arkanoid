#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Settings.h"

namespace Arkanoid
{
	class Application
	{
	public:
		Application();
		~Application() = default;
		void Run();
	private:
		std::unique_ptr<sf::RenderWindow> window;
	};

}
