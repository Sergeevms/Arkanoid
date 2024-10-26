#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"
#include "Settings.h"


namespace Arkanoid
{
	Application::Application()
	{
		Settings* settings = Settings::GetSettings();
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode(settings->screenWidth, settings->screenHeight), settings->gameName);
	}

	void Application::Run()
	{
		const unsigned seed = static_cast<unsigned int>(time(nullptr));
		srand(seed);

		sf::Clock clock;
		float lastTime = clock.getElapsedTime().asSeconds();
		Game* game = Game::GetGame();

		while (window->isOpen())
		{		
			std::vector<sf::Event> inputEvents;
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window->close();
				}
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::TextEntered)
				{
					inputEvents.push_back(event);
				}
			}

			float currentTime = clock.getElapsedTime().asSeconds();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			game->Update(deltaTime, inputEvents);
			
			window->clear();
			game->Draw(*window);
			window->display();

			if (game->IsGameShuttingDown())
			{
				window->close();
			}
		}
	}
}