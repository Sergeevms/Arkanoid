#include <assert.h>
#include "Game.h"
#include "Application.h"
#include "GameWorld.h"
#include "BaseState.h"
#include "PlayingState.h"
#include "MainMenuState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "GameWinnedState.h"

namespace Arkanoid
{
	Game::Game()
	{	
		GameWorld* world = GameWorld::GetWorld();
		stateStack.emplace_back(std::make_unique<MainMenuState>());
#ifdef _DEBUG
		assert(backGroundMusic.openFromFile(world->soundPath + "Clinthammer__Background_Music.wav"));
#else// _DEBUG
		backGroundMusic.openFromFile(world->soundPath + "Clinthammer__Background_Music.wav");
#endif
		backGroundMusic.setLoop(true);
		LoadSound(SoundType::OnKeyHit, "Owlstorm__Snake_hit.wav");
		LoadSound(SoundType::OnLose, "Maodin204__Lose.wav");
		LoadSound(SoundType::OnBallHit, "Theevilsocks__menu-hover.wav");
		LoadSound(SoundType::OnSessionStart, "Timgormly__Enter.wav");
	}

	bool Game::IsGameShuttingDown() const
	{
		return isShuttingDown;
	}

	void Game::Update(const float deltaTime, const std::vector<sf::Event>& inputEvents)
	{
		stateStack.rbegin()->get()->HandleInput(inputEvents);
		stateStack.rbegin()->get()->Update(deltaTime);
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
		if (newState != GameState::Playing)
		{
			backGroundMusic.stop();
		}

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
			if (GameWorld::GetWorld()->musicOn)
			{
				backGroundMusic.play();
			}
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
		case GameState::GameWinned:
		{
			stateStack.emplace_back(std::make_unique<GameWinnedState>());
			break;
		}
		}
	}

	void Game::ShutDown()
	{
		isShuttingDown = true;
	}

	void Game::PlaySound(const SoundType sound)
	{
		GameWorld* world = GameWorld::GetWorld();
		if (sounds.contains(sound) && world->soundOn)
		{
			sounds.at(sound).play();
		}
	}

	void Game::SetLastSessionScore(const int score)
	{
		lastSessionScore = score;
	}

	int Game::GetLastSessionScore() const
	{
		return lastSessionScore;
	}

	BaseState* Game::GetState() const
	{
		if (stateStack.empty())
		{
			return nullptr;
		}
		else
		{
			return stateStack.back().get();
		}
	}

	void Game::LoadSound(const SoundType type, std::string fileName)
	{
		GameWorld* world = GameWorld::GetWorld();
		soundBuffers.push_back(std::make_unique<sf::SoundBuffer>());
#ifdef _DEBUG
		assert((*soundBuffers.back()).loadFromFile(world->soundPath + fileName));
#else
		(*soundBuffers.back()).loadFromFile(world->soundPath + fileName);
#endif // _DEBUG
		sounds[type] = sf::Sound(*soundBuffers.back());
	}
}