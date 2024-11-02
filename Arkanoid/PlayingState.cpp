#include <assert.h>
#include <string>
#include "PlayingState.h"
#include "Application.h"
#include "Settings.h"
#include "Game.h"
#include "Utility.h"
#include "PlayingInputHandler.h"
#include "Platform.h"
#include "Ball.h"

namespace Arkanoid
{
	PlayingState::PlayingState() : BaseState()
	{
		gameObjects.push_back(std::make_shared<Platform>());
		gameObjects.push_back(std::make_shared<Ball>());
		inputHandler = std::make_unique<PlayingInputHandler>(std::dynamic_pointer_cast<Platform>(gameObjects.front()).get());
		Settings* settings = Application::GetSettings();

#ifdef _DEBUG
		assert(font.loadFromFile(settings->fontPath +  "Roboto-Regular.ttf"));
#else
		font.loadFromFile(settings->fontPath + "Roboto-Regular.ttf");
#endif // DEBUG

		scoreText.setFont(font);
		scoreText.setFillColor(sf::Color::Green);
		scoreText.setCharacterSize(40);
		
		std::wstring currentScore(L"Очки:\n");
		scoreText.setString(currentScore + std::to_wstring(scoreCount));
		SetOriginByRelative(scoreText, relativePositions.at(RelativePosition::TopRight));
		scoreText.setPosition({ settings->screenWidth - 10.f, 5.f });

		Application::GetInstance().GetGame()->SwitchMusicPlaying(true);
		ResetSessionDelay();
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		for (auto& object : gameObjects)
		{
			object->Draw(window);
		}
		window.draw(scoreText);
	}

	void PlayingState::Update(const float deltaTime)
	{
		if (isGameOvered)
		{
			Application::GetInstance().GetGame()->SwitchMusicPlaying(false);
		}
		else
		{
			if (sessionDelay <= 0.f)
			{
				for (auto& object : gameObjects)
				{
					object->Update(deltaTime);
				}
				std::dynamic_pointer_cast<Ball>(gameObjects[1])->CheckPlatformCollision(std::dynamic_pointer_cast<Platform>(gameObjects.front()).get());
			}
			else
			{
				sessionDelay -= deltaTime;
			}
		}
	}

	void PlayingState::ResetSessionDelay()
	{
		sessionDelay = Application::GetSettings()->sessionDelayTime;
	}
}