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
#include "Block.h"

namespace Arkanoid
{
	PlayingState::PlayingState() : BaseState()
	{
		gameObjects.push_back(std::make_shared<Platform>());
		inputHandler = std::make_unique<PlayingInputHandler>(std::dynamic_pointer_cast<Platform>(gameObjects.front()).get());
		gameObjects.push_back(std::make_shared<Ball>());
		Settings* settings = Application::GetSettings();

		for (int j = 0; j < settings->blockRowCount; j++)
		{
			for (int i = 0; i < settings->blocksInRow; i++)
			{
				sf::Vector2f position;
				//Getting coordinates of block center considering spacing between the blocks and one empty row at top
				position.x = settings->blockSpacing * (1 + i) + settings->blockSize.x * (i + 0.5f);
				position.y = settings->blockSpacing * (1 + j) + settings->blockSize.y * (j + 1 + 0.5f);
				gameObjects.push_back(std::make_shared<Block>(position));
			}
		}

		ResetSessionDelay();
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		for (auto& object : gameObjects)
		{
			object->Draw(window);
		}
	}

	void PlayingState::Update(const float deltaTime)
	{		
		if (sessionDelay <= 0.f)
		{
			//Updating objects
			for (auto& object : gameObjects)
			{
				object->Update(deltaTime);
			}

			//Collision checks
			Ball* ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]).get();
			for (int i = 2; i < gameObjects.size(); ++i)
			{
				if (ball->CheckCollision(gameObjects[i].get()))
				{
					if (std::dynamic_pointer_cast<Block>(gameObjects[i]))
					{
						std::swap(gameObjects[i], gameObjects.back());
						gameObjects.pop_back();
						Application::GetInstance().GetGame()->PlaySound(SoundType::OnSnakeHit);
					}
				}
			}

			//Win & loose conditions check
			if (gameObjects.size() <= 2)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::GameWinned);
			}
			Platform* platform = std::dynamic_pointer_cast<Platform>(gameObjects.front()).get();
			bool platformCollided = ball->CheckCollision(platform);
			if (platformCollided)
			{
				Application::GetInstance().GetGame()->PlaySound(SoundType::OnSnakeHit);
			}
			if (ball->GetRect().top + ball->HalfSize().y > platform->GetRect().top && !platformCollided)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::Records);
			}
		}
		else
		{
			sessionDelay -= deltaTime;
		}		
	}

	void PlayingState::ResetSessionDelay()
	{
		sessionDelay = Application::GetSettings()->sessionDelayTime;
	}
}