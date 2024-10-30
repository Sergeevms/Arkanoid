#include <assert.h>
#include <string>
#include "PlayingState.h"
#include "Application.h"
#include "Settings.h"
#include "Game.h"
#include "Utility.h"
#include "PlayingInputHandler.h"

namespace Arkanoid
{
	PlayingState::PlayingState() : BaseState()
	{		
		inputHandler = std::make_unique<PlayingInputHandler>();
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
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		window.draw(scoreText);
	}

	void PlayingState::Update(const float deltaTime)
	{
		Settings* settings = Application::GetSettings();
		Game* game = Application::GetInstance().GetGame();
		if (isGameOvered)
		{
			Application::GetInstance().GetGame()->SwitchMusicPlaying(false);
		}
		else
		{
			Game* game = Application::GetInstance().GetGame();
		}
	}
}