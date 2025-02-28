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
#include "Bonus.h"
#include "BonusFactory.h"
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
			blockFactories.emplace(blockType, BlockFactory::CreateFactory(blockType));
		}

		for (auto& bonusType : GameWorld::GetWorld()->avaliableBonuses)
		{
			bonusFactories.emplace(bonusType, BonusFactory::CreateFactory(bonusType));
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

			//Collision checks
			Ball* ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]).get();

			bool needInvertX = false;
			bool needInvertY = false;
			bool hasBrokeOneBlock = false;
			auto blockCollisionCheckFunctor =
				[ball, &hasBrokeOneBlock, &needInvertX, &needInvertY, this](auto object)
				{
					if (auto block = std::dynamic_pointer_cast<Block> (object))
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
						if (block->IsBroken())
						{
							if (auto bonus = BonusFactory::TryToCreateRandomBonus(bonusFactories, block->GetPosition()))
							{
								gameObjects.emplace_back(bonus);
								bonus->AddObserver(weak_from_this());
							}
							objectsToDestroy.push_back(block);
						}
					}
				};

			for (auto& block : gameObjects)
			{
				blockCollisionCheckFunctor(block);
			}

			if (needInvertX)
			{
				ball->InvertX();
			}

			if (needInvertY)
			{
				ball->InvertY();
			}

			auto platform = std::dynamic_pointer_cast<Platform>(gameObjects.front());
			for (auto& object : gameObjects)
			{
				platform->CheckCollision(object.get());
			}
		}
		else
		{
			sessionDelay -= deltaTime;
		}
		
		//removing GameObjects needed to be destroyed
		gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [this](auto& object)
			{
				return std::find(objectsToDestroy.begin(), objectsToDestroy.end(), object) != objectsToDestroy.end();
			}), gameObjects.end());
		objectsToDestroy.clear();
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
			ClearGameObjects<Block>();
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
		else if (auto ball = std::dynamic_pointer_cast<Ball>(observable); ball)
		{
			if (ball->GetPosition().y > gameObjects.front()->GetRect().top)
			{
				auto game = Application::GetInstance().GetGame();
				game->SetLastSessionScore(currentScore);
				game->LooseGame();
			}
		}
		else if (auto bonus = std::dynamic_pointer_cast<Bonus>(observable); bonus)
		{
			if (bonus->IsActivated())
			{
				for (auto& object : gameObjects)
				{
					bonus->Apply(object);
				}
			}
			else
			{
				objectsToDestroy.push_back(bonus);
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
			for (auto& object : gameObjects)
			{
				if (auto block = std::dynamic_pointer_cast<Block>(object))
				{
					stateSave->blockTypes.push_back(block->GetBlockType());
					stateSave->blockSaves.push_back(std::dynamic_pointer_cast<BlockSave>(block->SaveState()));
				}
				if (auto bonus = std::dynamic_pointer_cast<Bonus>(object))
				{
					stateSave->bonusTypes.push_back(bonus->GetType());
					stateSave->bonusSaves.push_back(std::dynamic_pointer_cast<BonusSave>(bonus->SaveState()));
				}
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
			ClearGameObjects<Block>();
			CreateBlocks(stateSave);
			ClearGameObjects<Bonus>();
			for (size_t i = 0; i < stateSave->bonusTypes.size(); ++i)
			{
				gameObjects.push_back(CreateBonusByType(bonusFactories, stateSave->bonusTypes[i]));
				std::dynamic_pointer_cast<IObservable>(gameObjects.back())->AddObserver(weak_from_this());
				gameObjects.back()->LoadState(stateSave->bonusSaves[i]);
			}
		}
	}

	void PlayingState::CreateBlocks(std::shared_ptr<PlayingStateSave> stateSave)
	{
		const GameWorld* world = GameWorld::GetWorld();
		
		for (auto& pair : blockFactories)
		{
			pair.second->ClearBreakableCounter();
		}
		breakableBlocksCount = 0;
		auto self = weak_from_this();
		if (stateSave)
		{
			for (int i = 0; i < stateSave->blockSaves.size(); ++i)
			{
				gameObjects.push_back(blockFactories.at(stateSave->blockTypes[i])->CreateBlock());
				std::dynamic_pointer_cast<Block>(gameObjects.back())->AddObserver(self);
				gameObjects.back()->LoadState(stateSave->blockSaves[i]);
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
				gameObjects.push_back(blockFactories.at(block.second)->CreateBlock(position));
				std::dynamic_pointer_cast<Block>(gameObjects.back())->AddObserver(self);
			}
		}
		for (auto& pair : blockFactories)
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
		ostream << bonusSaves.size() << " ";
		for (auto& bonusType : bonusTypes)
		{
			ostream << static_cast<int>(bonusType) << " ";
		}
		for (auto& bonus : bonusSaves)
		{
			bonus->SaveToFile(ostream);
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

		size_t bonusCount = 0;
		istream >> bonusCount;
		bonusSaves.clear();
		bonusSaves.reserve(bonusCount);
		bonusTypes.clear();
		bonusTypes.reserve(bonusCount);
		for (size_t i = 0; i < bonusCount; ++i)
		{
			int bonusType = 0;
			istream >> bonusType;
			bonusTypes.push_back(static_cast<BonusType>(bonusType));
			bonusSaves.push_back(CreateEmptySaveByBonusType(bonusTypes.back()));
		}
		for (auto& bonusSave : bonusSaves)
		{
			bonusSave->LoadFromFile(istream);
		}

	}

	PlayingStateSave::PlayingStateSave()
	{
		ball = std::make_shared<BallSave>();
		platform = std::make_shared<GameObjectSave>();
	}
}