#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"
#include "BaseState.h"


namespace Arkanoid
{
	Application::Application() :
		window(sf::VideoMode(GameWorld::GetWorld()->screenWidth, GameWorld::GetWorld()->screenHeight), GameWorld::GetWorld()->gameName), game(std::make_unique<Game>())
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

			game->UpdateGame(GameWorld::GetWorld()->timePerFrame, window);

			const float endTime = clock.getElapsedTime().asSeconds();
			const float deltaTime = endTime - startTime;

			if (deltaTime < GameWorld::GetWorld()->timePerFrame)
			{
				sf::sleep(sf::seconds(GameWorld::GetWorld()->timePerFrame - deltaTime));
			}			
		}
	}
}