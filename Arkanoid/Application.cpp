#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"
#include "BaseState.h"


namespace Arkanoid
{
	Application::Application() :
		window(sf::VideoMode(GetSettings()->screenWidth, GetSettings()->screenHeight), GetSettings()->gameName), game(std::make_unique<Game>())
	{
	}

	Application& Application::GetInstance()
	{
		static Application application;
		return application;
	}

	Game* Application::GetGame()
	{
		return game.get();
	}

	void Application::Run()
	{
		const unsigned seed = static_cast<unsigned int>(time(nullptr));
		srand(seed);

		sf::Clock clock;
		Game* game = Application::GetInstance().GetGame();

		while (window.isOpen())
		{
			const float startTime = clock.getElapsedTime().asSeconds();

			std::vector<sf::Event> inputEvents;
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::TextEntered)
				{
					inputEvents.push_back(event);
				}
			}

			game->Update(GetSettings()->timePerFrame, inputEvents);
			
			window.clear();
			game->Draw(window);
			window.display();

			if (game->IsGameShuttingDown())
			{
				window.close();
			}

			const float endTime = clock.getElapsedTime().asSeconds();
			const float deltaTime = endTime - startTime;

			if (deltaTime < GetSettings()->timePerFrame)
			{
				sf::sleep(sf::seconds(GetSettings()->timePerFrame - deltaTime));
			}			
		}
	}
}