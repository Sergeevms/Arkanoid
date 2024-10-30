#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Game.h"


namespace Arkanoid
{
	class Application
	{
	public:
		Application(Application&) = delete;
		Application& operator=(Application&) = delete;
		static Application& GetInstance();
		Game* GetGame();
		static Settings* GetSettings() {
			static std::unique_ptr<Settings> settings = std::make_unique<Settings>();
			return settings.get();
		};
		void Run();
	private:
		Application();
		~Application() = default;
		sf::RenderWindow window;
		std::unique_ptr<Game> game;
	};

}
