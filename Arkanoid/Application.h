#pragma once
#include <SFML/Graphics.hpp>
#include "GameWorld.h"
#include "Game.h"


namespace Arkanoid
{
	class Application
	{
	public:
		Application(const Application&) = delete;
		Application operator=(const Application&) = delete;
		static Application& GetInstance();
		Game* GetGame();
		void Run();
	private:
		Application();
		~Application() = default;
		sf::RenderWindow window;
		std::unique_ptr<Game> game;
	};

}
