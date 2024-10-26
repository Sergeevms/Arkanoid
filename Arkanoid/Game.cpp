#include <assert.h>
#include "Game.h"
#include "Settings.h"
#include "BaseState.h"
#include "PlayingState.h"
#include "MainMenuState.h"
#include "RecordsState.h"
#include "PauseState.h"

namespace SnakeGame
{
	Game* Game::game = nullptr;

	Game::Game()
	{	
		Settings* settings = Settings::GetSettings();
		stateStack.emplace_back(std::make_unique<MainMenuState>());
#ifdef _DEBUG
		assert(backGroundMusic.openFromFile(settings->soundPath + "Clinthammer__Background_Music.wav"));
#else// _DEBUG
		backGroundMusic.openFromFile(settings->soundPath + "Clinthammer__Background_Music.wav");
#endif
		backGroundMusic.setLoop(true);
		loadSound(SoundType::OnKeyHit, "Owlstorm__Snake_hit.wav");
		loadSound(SoundType::OnLose, "Maodin204__Lose.wav");
		loadSound(SoundType::OnSnakeHit, "Theevilsocks__menu-hover.wav");
		loadSound(SoundType::OnSessionStart, "Timgormly__Enter.wav");
	}

	bool Game::IsGameShuttingDown() const
	{
		return isShuttingDown;
	}

	void Game::Update(const float deltaTime, const std::vector<sf::Event>& inputEvents)
	{
		stateStack.rbegin()->get()->HandleInput(inputEvents);
		if (dynamic_cast<PauseState*>(stateStack.rbegin()->get()))
		{
			stateStack.rbegin()->get()->Update(deltaTime);
		}
		else
		{
			for (auto& state : stateStack)
			{
				state.get()->Update(deltaTime);
			}
		}
	}

	void Game::Draw(sf::RenderWindow& window) const
	{
		for (auto& state : stateStack)
		{
			state->Draw(window);
		}
	}

	void Game::SwitchToState(GameState newState)
	{
		switch (newState)
		{
		case GameState::MainMenu:
		{			
			stateStack.clear();
			stateStack.emplace_back(std::make_unique<MainMenuState>());
			break;
		}
		case GameState::Playing:
		{
			if (dynamic_cast<PauseState*>(stateStack.back().get()))
			{
				stateStack.pop_back();
			}
			else
			{
				stateStack.clear();
				stateStack.emplace_back(std::make_unique<PlayingState>());
			}
			break;
		}
		case GameState::Records:
		{
			if (dynamic_cast<PlayingState*>(stateStack.back().get()))
			{
				stateStack.emplace_back(std::make_unique<RecordsState>(true));
			}
			else
			{
				stateStack.emplace_back(std::make_unique<RecordsState>(false));
			}
			break;
		}
		case GameState::Pause:
		{
			stateStack.emplace_back(std::make_unique<PauseState>());
			break;
		}
		}
	}

	void Game::ShutDown()
	{
		isShuttingDown = true;
	}

	void Game::SwitchMusicPlaying(bool playing)
	{
		Settings* settings = Settings::GetSettings();
		if (playing && settings->musicOn)
		{
			backGroundMusic.play();
		}
		else
		{
			backGroundMusic.stop();
		}
	}

	void Game::PlaySound(const SoundType sound)
	{
		Settings* settings = Settings::GetSettings();
		if (sounds.contains(sound) && settings->soundOn)
		{
			sounds.at(sound).play();
		}
	}

	void Game::SetLastSessionScore(const int score)
	{
		lastSessionScore = score;
	}

	int Game::GetLastSessionScore()
	{
		return lastSessionScore;
	}

	void Game::loadSound(const SoundType type, std::string fileName)
	{
		Settings* settings = Settings::GetSettings();
		soundBuffers.push_back(std::make_unique<sf::SoundBuffer>());
#ifdef _DEBUG
		assert((*soundBuffers.back()).loadFromFile(settings->soundPath + fileName));
#else
		(*soundBuffers.back()).loadFromFile(settings->soundPath + fileName);
#endif // _DEBUG
		sounds[type] = sf::Sound(*soundBuffers.back());
	}
}