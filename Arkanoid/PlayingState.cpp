#include <assert.h>
#include <string>
#include "PlayingState.h"
#include "Application.h"
#include "GameWorld.h"
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
		GameWorld* world = GameWorld::GetWorld();		
		CreateBlocks();
		ResetSessionDelay();
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		auto drawFunctor = [&window](auto object)
			{
				object->Draw(window);
			};
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunctor);
		std::for_each(blocks.begin(), blocks.end(), drawFunctor);
	}

	void PlayingState::Update(const float deltaTime)
	{		
		if (sessionDelay <= 0.f)
		{
			//Updating objects
			auto updateFunctor = [deltaTime](auto object)
				{
					object->Update(deltaTime);
				};
			std::for_each(gameObjects.begin(), gameObjects.end(), updateFunctor);
			std::for_each(blocks.begin(), blocks.end(), updateFunctor);


			//Collision checks
			Ball* ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]).get();

			bool needInvertX = false;
			bool needInvertY = false;
			bool hasBrokeOneBlock = false;
			blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
				[ball, &hasBrokeOneBlock, &needInvertX, &needInvertY, this](auto block)
				{
					if ((!hasBrokeOneBlock) && (block->CheckCollision(ball)))
					{
						Application::GetInstance().GetGame()->PlaySound(SoundType::OnBallHit);
						hasBrokeOneBlock = true;
						sf::Vector2f ballPosition = ball->GetPosition();
						sf::FloatRect blockRect = block->GetRect();

						if (block->IsBallReboundable())
						{
							GetBallInverse(ballPosition, blockRect, needInvertX, needInvertY);
						}
					}
					return block->IsBroken();
				}),
				blocks.end());

			if (needInvertX)
			{
				ball->InvertX();
			}

			if (needInvertY)
			{
				ball->InvertY();
			}

			Platform* platform = std::dynamic_pointer_cast<Platform>(gameObjects.front()).get();
			bool platformCollided = platform->CheckCollision(ball);
			if (platformCollided)
			{
				Application::GetInstance().GetGame()->PlaySound(SoundType::OnBallHit);
			}

			//Win & loose conditions check
			size_t unbreakbleBloks = std::count_if(blocks.begin(), blocks.end(),
				[](auto block)
				{
					if (std::dynamic_pointer_cast<UnbreakbleBlock>(block))
					{
						return true;
					}
					return false;
				}
			);

			if (blocks.size() <= unbreakbleBloks)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::GameWinned);
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
		sessionDelay = GameWorld::GetWorld()->sessionDelayTime;
	}

	void PlayingState::CreateBlocks()
	{
		const GameWorld* world = GameWorld::GetWorld();
		for (int j = 0; j < world->blockRowCount; j++)
		{
			for (int i = 0; i < world->blocksInRow; i++)
			{
				sf::Vector2f position;
				//Getting coordinates of block center considering spacing between the blocks and one two row at top
				position.x = world->blockSpacing * (1 + i) + world->blockSize.x * (i + 0.5f);
				position.y = world->blockSpacing * (2 + j) + world->blockSize.y * (j + 2 + 0.5f);
				blocks.push_back(CreateRandomBlock(position));
			}
		}
	}

	void PlayingState::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseX, bool& needInverseY)
	{
		if (ballPos.y > blockRect.top + blockRect.height)
		{
			needInverseY = true;
		}

		if (ballPos.x < blockRect.left || ballPos.x > blockRect.left + blockRect.width)
		{
			needInverseX = true;
		}
	}
}