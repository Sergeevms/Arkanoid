#include <assert.h>
#include <string>
#include <memory>
#include "PlayingState.h"
#include "Application.h"
#include "GameWorld.h"
#include "Game.h"
#include "Utility.h"
#include "PlayingInputHandler.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "BlockFactory.h"
#include "LevelLoader.h"

namespace Arkanoid
{
	PlayingState::PlayingState() : BaseState(), levelLoader(std::make_unique<LevelLoader>())
	{
		gameObjects.push_back(std::make_shared<Platform>());
		inputHandler = std::make_unique<PlayingInputHandler>(std::dynamic_pointer_cast<Platform>(gameObjects.front()).get());
		gameObjects.push_back(std::make_shared<Ball>());
		for (auto& blockType : GameWorld::GetWorld()->availiableBlockTypes)
		{
			factories.emplace(blockType, CreateFactory(blockType));
		}
		scoreTextStyle.Init("Roboto-Regular.ttf", sf::Color::Green);
		scoreText.SetStyle(&scoreTextStyle);
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		auto drawFunctor = [&window](auto object)
			{
				object->Draw(window);
			};
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunctor);
		std::for_each(blocks->begin(), blocks->end(), drawFunctor);		
		scoreText.Draw(window);
	}

	void PlayingState::Update(const float deltaTime)
	{
		scoreText.setString(L"Очки:\n" + std::to_wstring(currentScore));
		scoreText.PresetPosition({ GameWorld::GetWorld()->screenWidth - 3.f, 3.f }, Orientation::Vertical, Alignment::Max);
		if (sessionDelay <= 0.f)
		{
			//Updating objects
			auto updateFunctor = [deltaTime](auto object)
				{
					object->Update(deltaTime);
				};
			std::for_each(gameObjects.begin(), gameObjects.end(), updateFunctor);
			std::for_each(blocks->begin(), blocks->end(), updateFunctor);


			//Collision checks
			Ball* ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]).get();

			bool needInvertX = false;
			bool needInvertY = false;
			bool hasBrokeOneBlock = false;
			std::shared_ptr<std::vector<std::shared_ptr<Block>>> blocks = PlayingState::blocks;
			blocks->erase(std::remove_if(blocks->begin(), blocks->end(),
				[ball, &hasBrokeOneBlock, &needInvertX, &needInvertY, this](auto block)
				{
					if ((!hasBrokeOneBlock) && (block->CheckCollision(ball)))
					{
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
				blocks->end());

			if (needInvertX)
			{
				ball->InvertX();
			}

			if (needInvertY)
			{
				ball->InvertY();
			}

			std::dynamic_pointer_cast<Platform>(gameObjects.front())->CheckCollision(ball);
		}
		else
		{
			sessionDelay -= deltaTime;
		}
	}

	void PlayingState::Init()
	{
		auto self = weak_from_this();
		if (auto ball = std::dynamic_pointer_cast<IObservable>(gameObjects[1]); ball)
		{
			ball->AddObserver(weak_from_this());			
		}
		LoadNextLevel();
	}

	void PlayingState::ResetSessionDelay()
	{
		sessionDelay = GameWorld::GetWorld()->sessionDelayTime;
	}

	void PlayingState::LoadNextLevel()
	{
		if (nextLevel >= levelLoader->GetLevelCount())
		{
			auto game = Application::GetInstance().GetGame();
			game->SetLastSessionScore(currentScore);
			game->WinGame();
		}
		else
		{
			blocks = std::make_shared <std::vector<std::shared_ptr<Block>>>();
			CreateBlocks();
			for (auto& object : gameObjects)
			{
				object->Reset();
			}
			ResetSessionDelay();
			++nextLevel;
		}
	}

	void PlayingState::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto block = std::dynamic_pointer_cast<Block>(observable); block)
		{
			currentScore += block->GetScore();
			if (--breakableBlocksCount <= 0)
			{
				Application::GetInstance().GetGame()->LoadNextLevel();
			};
		}
		else
		if (auto ball = std::dynamic_pointer_cast<Ball>(observable); ball)
		{
			if (ball->GetPosition().y > gameObjects.front()->GetRect().top)
			{
				auto game = Application::GetInstance().GetGame();
				game->SetLastSessionScore(currentScore);
				game->LooseGame();
			}
		}
	}

	std::shared_ptr<ISave> PlayingState::SaveState() const
	{
		auto save = std::make_shared<PlayingStateSave>();
		SaveState(save);
		return save;
	}

	void PlayingState::SaveState(std::shared_ptr<ISave> save) const
	{
		if (auto stateSave = std::dynamic_pointer_cast<PlayingStateSave>(save))
		{
			stateSave->currentScore = currentScore;
			stateSave->nextLevel = nextLevel;
			stateSave->platform = std::dynamic_pointer_cast<GameObjectSave>(gameObjects[0]->SaveState());
			stateSave->ball = std::dynamic_pointer_cast<BallSave>(gameObjects[1]->SaveState());
			stateSave->blockTypes.clear();
			stateSave->blockSaves.clear();
			for (auto& block : *blocks)
			{
				stateSave->blockTypes.push_back(block->GetBlockType());
				stateSave->blockSaves.push_back(std::dynamic_pointer_cast<BlockSave>(block->SaveState()));
			}
		}
	}

	void PlayingState::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto stateSave = std::dynamic_pointer_cast<PlayingStateSave>(save))
		{
			currentScore = stateSave->currentScore;
			nextLevel = stateSave->nextLevel;
			gameObjects[0]->LoadState(stateSave->platform);
			gameObjects[1]->LoadState(stateSave->ball);
			blocks->clear();
			CreateBlocks(stateSave);
		}
	}

	void PlayingState::CreateBlocks(std::shared_ptr<PlayingStateSave> stateSave)
	{
		const GameWorld* world = GameWorld::GetWorld();
		
		for (auto& pair : factories)
		{
			pair.second->ClearBreakableCounter();
		}
		breakableBlocksCount = 0;
		auto self = weak_from_this();
		if (stateSave)
		{
			for (int i = 0; i < stateSave->blockSaves.size(); ++i)
			{
				blocks->push_back(factories.at(stateSave->blockTypes[i])->CreateBlock());
				blocks->back()->LoadState(stateSave->blockSaves[i]);
				blocks->back()->AddObserver(self);
			}
		}
		else
		{
			auto& level = levelLoader->GetLevel(nextLevel);
			for (auto& block : level.blocks)
			{
				sf::Vector2f position;
				//Getting coordinates of block center considering spacing between the blocks and one two row at top
				position.x = world->blockSpacing * (1 + block.first.x) + world->blockSize.x * (block.first.x + 0.5f);
				position.y = world->blockSpacing * (2 + block.first.y) + world->blockSize.y * (block.first.y + 2 + 0.5f);
				blocks->push_back(factories.at(block.second)->CreateBlock(position));
				blocks->back()->AddObserver(self);
			}
		}
		for (auto& pair : factories)
		{
			breakableBlocksCount += pair.second->GetBreakableBlockCount();
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

	void PlayingStateSave::SaveToFile(std::ofstream& ostream) const
	{
		ostream << nextLevel << " " << currentScore << " ";
		platform->SaveToFile(ostream);
		ball->SaveToFile(ostream);
		ostream << blockSaves.size() << " ";
		for (auto& blockType : blockTypes)
		{
			ostream << static_cast<int>(blockType) << " ";
		}
		for (auto& block : blockSaves)
		{
			block->SaveToFile(ostream);
		}
	}

	void PlayingStateSave::LoadFromFile(std::ifstream& istream)
	{		
		istream >> nextLevel >> currentScore;
		platform->LoadFromFile(istream);
		ball->LoadFromFile(istream);
		size_t blocksCount = 0;
		istream	>> blocksCount;
		blockSaves.clear();
		blockSaves.reserve(blocksCount);
		blockTypes.clear();
		blockTypes.reserve(blocksCount);
		for (size_t i = 0; i < blocksCount; ++i)
		{
			int blockType = 0;
			istream >> blockType;
			blockTypes.push_back(static_cast<BlockType>(blockType));
			blockSaves.push_back(CreateEmptySaveByBlockType(blockTypes.back()));
		}
		for (auto& blockSave : blockSaves)
		{
			blockSave->LoadFromFile(istream);
		}
	}

	PlayingStateSave::PlayingStateSave()
	{
		ball = std::make_shared<BallSave>();
		platform = std::make_shared<GameObjectSave>();
	}
}