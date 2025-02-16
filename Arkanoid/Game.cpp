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
		stateStack.emplace_back(std::make_shared<MainMenuState>());
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
		return stateStack.empty();
	}

	void Game::Update(const float deltaTime)
	{
		stateStack.rbegin()->get()->Update(deltaTime);
	}

	void Game::HandleInputEvents(sf::RenderWindow& window)
	{
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
		stateStack.rbegin()->get()->HandleInput(inputEvents);
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
			stateStack.emplace_back(std::make_shared<MainMenuState>());
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
				stateStack.emplace_back(std::make_shared<PlayingState>());
				stateStack.back()->Init();
			}
			break;
		}
		case GameState::Records:
		{
			if (dynamic_cast<PlayingState*>(stateStack.back().get()))
			{
				stateStack.emplace_back(std::make_shared<RecordsState>(true));
			}
			else
			{
				stateStack.emplace_back(std::make_shared<RecordsState>(false));
			}
			break;
		}
		case GameState::Pause:
		{
			stateStack.emplace_back(std::make_shared<PauseState>());
			break;
		}
		case GameState::GameWinned:
		{
			stateStack.emplace_back(std::make_shared<GameWinnedState>());
			break;
		}
		case GameState::None:
		{
			stateStack.clear();
			break;
		}
		}
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

	void Game::StartGame()
	{
		SwitchToState(GameState::Playing);
	}

	void Game::PauseGame()
	{
		SwitchToState(GameState::Pause);
	}

	void Game::WinGame()
	{
		SwitchToState(GameState::GameWinned);
	}

	void Game::LooseGame()
	{
		PlaySound(SoundType::OnLose);
		SwitchToState(GameState::Records);
	}

	void Game::UpdateGame(const float deltaTime, sf::RenderWindow& window)
	{
		HandleInputEvents(window);
		if (IsGameShuttingDown())
		{
			window.close();
		}		
		else
		{
			Update(deltaTime);
			window.clear();
			Draw(window);
			window.display();
		}
	}

	void Game::ToMainMenu()
	{
		SwitchToState(GameState::MainMenu);
	}

	void Game::ShowRecords()
	{
		SwitchToState(GameState::Records);
	}

	void Game::LoadNextLevel()
	{
		auto playingState = dynamic_cast<PlayingState*>(stateStack.back().get());
		if (playingState)
		{
			playingState->LoadNextLevel();
		}
	}

	void Game::Shutdown()
	{
		SwitchToState(GameState::None);
	}

	void Game::LoadSavedGame()
	{
		SwitchToState(GameState::Playing);
		std::ifstream input(GameWorld::GetWorld()->saveFile);
		if (input.is_open())
		{
			auto stateSave = std::make_shared<PlayingStateSave>();
			stateSave->LoadFromFile(input);
			std::dynamic_pointer_cast<PlayingState>(stateStack.back())->LoadState(stateSave);
			input.close();
		}
	}

	void Game::SaveGameAndGoToMenu()
	{
		for (auto& state : stateStack)
		{
			if (auto playingState = std::dynamic_pointer_cast<PlayingState>(state))
			{
				std::ofstream output(GameWorld::GetWorld()->saveFile);
				if (output.is_open())
				{
					auto stateSave = playingState->SaveState();
					stateSave->SaveToFile(output);
					output.close();
				}
			}
		}
		SwitchToState(GameState::MainMenu);
	}

	void Game::PlaySoundOnKeyHit()
	{
		PlaySound(SoundType::OnKeyHit);
	}

	void Game::PlaySoundOnBallHit()
	{
		PlaySound(SoundType::OnBallHit);
	}

	void Game::PlaySoundOnSessionStart()
	{
		PlaySound(SoundType::OnSessionStart);
	}
}