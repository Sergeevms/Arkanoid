#include <assert.h>
#include <string>
#include "PlayingState.h"
#include "Settings.h"
#include "Apple.h"
#include "Game.h"
#include "PlayingInputHandler.h"
#include "Snake.h"
#include "Wall.h"

namespace SnakeGame
{
	PlayingState::PlayingState() : BaseState(), map(), snake(this, &map),
		delayBeforeMoving(Settings::GetSettings()->movingDelayOnStart)
	{
		appleFactory = std::make_unique<AppleFactory>();
		inputHandler = std::make_unique<PlayingInputHandler>(&snake, this);
		Settings* settings = Settings::GetSettings();
		map.LoadFromFile(settings->selectedLevel);
		map.CreateSavedLvl();

		if (map.ValidCell(map.GetLoadedApplePosition()))
		{
			currentApple = appleFactory.get()->GenerateNewApple(&map, map.GetLoadedApplePosition());
		}
		else
		{
			currentApple = appleFactory.get()->GenerateNewApple(&map);		
		}

		map.EmplaceMapObject(currentApple);

		if (std::dynamic_pointer_cast<GoldenApple>(currentApple))
		{
			timeTillGoldenAppleDisapear = settings->goldenAppleLifeTime;
		}

		if (map.ValidCell(map.GetLoadedSnakeHeadPosition()))
		{
			snake.LoadFromCharMap(map.GetCharMap(), map.GetLoadedSnakeHeadPosition());
		}
		else
		{
			snake.GenerateSnake(&map);
		}

		if (settings->randomWallsOn)
		{
			map.GenerateRandomWalls();
		}

		if (settings->temporaryWallsOn)
		{
			temporaryWallsPlaced = true;
			temporaryWallsTimer = settings->temporaryWallLifeTime;
		}

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

		Game::GetGame()->SwitchMusicPlaying(true);

		keepSnakeMovingTime = 0.f;
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		map.Draw(window);
		window.draw(scoreText);
	}

	void PlayingState::Update(const float deltaTime)
	{
		keepSnakeMovingTime -= deltaTime;
		Settings* settings = Settings::GetSettings();
		Game* game = Game::GetGame();
		if (isGameOvered)
		{
			Game::GetGame()->SwitchMusicPlaying(false);
			if (keepSnakeMovingTime > 0.f)
			{
				snake.Update(deltaTime);
			}
		}
		else
		{
			Game* game = Game::GetGame();
			if (delayBeforeMoving <= 0.f)
			{
				if (std::dynamic_pointer_cast<GoldenApple>(currentApple))
				{
					timeTillGoldenAppleDisapear -= deltaTime;
					if (timeTillGoldenAppleDisapear <= 0.f)
					{
						map.RemoveMapObject(currentApple);
						GenerateApple();
					}
				}
				snake.Update(deltaTime);

				if (settings->temporaryWallsOn)
				{
					temporaryWallsTimer -= deltaTime;
					if (temporaryWallsTimer <= 0.f)
					{						
						if (temporaryWallsPlaced)
						{
							map.SetTemporaryWallsOpacity(0);
							temporaryWallsPlaced = false;
							map.RemoveTemporaryWalls();
						}
						else
						{
							map.SetTemporaryWallsOpacity(255);
							temporaryWallsPlaced = true;
							map.EmplaceTemporaryWalls();
						}
						temporaryWallsTimer = settings->temporaryWallLifeTime;
					}
					else
					{
						if (temporaryWallsTimer <= settings->temporaryWallFadingTime)
						{
							float currentPart = (settings->temporaryWallFadingTime - temporaryWallsTimer) / settings->temporaryWallFadingTime;
							int opacity = static_cast<int>(temporaryWallsPlaced ? 255 - currentPart * settings->fadingBorderValue :	0 + currentPart * settings->fadingBorderValue);
							map.SetTemporaryWallsOpacity(opacity);
						}
					}					
				}

				if (!sessionStarted)
				{
					sessionStarted = true;
					game->PlaySound(SoundType::OnSessionStart);
				}
			}
			else
			{
				delayBeforeMoving -= deltaTime;
			}
			std::wstring currentScore(L"Очки:\n");
			scoreText.setString(currentScore + std::to_wstring(scoreCount));
			SetOriginByRelative(scoreText, relativePositions.at(RelativePosition::TopRight));
			if (!map.HaveEmptyCells())
			{
				game->SetLastSessionScore(scoreCount);
				game->SwitchToState(GameState::Records);
				isGameOvered = true;
			}
		}
	}

	void PlayingState::resetMovingDelay()
	{
		delayBeforeMoving = Settings::GetSettings()->movingDelayOnStart;
		sessionStarted = false;
	}

	bool PlayingState::CheckColition(sf::Vector2i& cell)
	{
		Game* game = Game::GetGame();
		Settings* settings = Settings::GetSettings();

		MapObject* collisionObject = map.GetObject(cell);

		if (dynamic_cast<Apple*>(collisionObject))
		{
			game->PlaySound(SoundType::OnSnakeHit);
			snake.AddNewBody();
			GenerateApple();
			scoreCount += static_cast<int>((dynamic_cast<GoldenApple*> (collisionObject) ? 
				settings->goldenAppleScoreModifier : 1.f) * settings->difficultyToScore[settings->GetCurrentDifficulty()]);
			keepSnakeMovingTime = settings->GetTimeOnCell() / (snake.IsPoisioned() ? settings->poisonedSpeedModifire : 1.f);
			if (dynamic_cast<DisorientApple*>(collisionObject))
			{
				snake.GetDisoriented();
			}
			if (dynamic_cast<PoisendApple*>(collisionObject))
			{
				snake.GetPoisioned();
			}
			map.RemoveMapObject(cell);
			return true;
		}
		else if (IsCollisionOveringGame(collisionObject))
		{
			isGameOvered = true;
			game->PlaySound(SoundType::OnLose);
			game->SetLastSessionScore(scoreCount);
			game->SwitchToState(GameState::Records);
			return false;
		}
		else
		{
			return true;
		}
	}

	void PlayingState::GenerateApple()
	{
		if (map.GetEmptyCellCount() > 1)
		{
			currentApple = appleFactory.get()->GenerateNewApple(&map);
			map.EmplaceMapObject(currentApple);
			if (std::dynamic_pointer_cast<GoldenApple>(currentApple))
			{
				timeTillGoldenAppleDisapear = Settings::GetSettings()->goldenAppleLifeTime;
			}
		}
	}
}