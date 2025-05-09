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
#include "ScoreCounter.h"

namespace Arkanoid
{
	PlayingState::PlayingState() : BaseState(), levelLoader(std::make_unique<LevelLoader>()), scoreImage(std::make_shared<ScoreImage>())
	{
		gameObjects.push_back(std::make_shared<Platform>());
		inputHandler = std::make_unique<PlayingInputHandler>(std::dynamic_pointer_cast<Platform>(gameObjects.back()).get());
		gameObjects.push_back(std::make_shared<Ball>());
		for (auto& blockType : GameWorld::GetWorld()->availiableBlockTypes)
		{
			blockFactories.emplace(blockType, BlockFactory::CreateFactory(blockType));
		}

		for (auto& bonusType : GameWorld::GetWorld()->avaliableBonuses)
		{
			bonusFactories.emplace(bonusType, BonusFactory::CreateFactory(bonusType));
		}
	}

	void PlayingState::Draw(sf::RenderWindow& window) const
	{
		auto drawFunctor = [&window](auto object)
			{
				object->Draw(window);
			};
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunctor);
		scoreImage->Draw(window);
	}

	void PlayingState::Update(const float deltaTime)
	{
		if (sessionDelay <= 0.f)
		{
			std::shared_ptr<IBallObject> ball;
			std::shared_ptr<IPlatformObject> platform;
			//Updating objects & selecting ball and platform
			auto updateFunctor = [deltaTime, &ball, &platform](auto object)
				{					
					object->Update(deltaTime);
					if (auto ballCandidate = std::dynamic_pointer_cast<IBallObject>(object))
					{
						ball = ballCandidate;
					}
					else if (auto platformCandidate = std::dynamic_pointer_cast<IPlatformObject>(object))
					{
						platform = platformCandidate;
					}
				};
			std::for_each(gameObjects.begin(), gameObjects.end(), updateFunctor);

			//Collision checks

			bool needInvertX = false;
			bool needInvertY = false;
			bool hasBrokeOneBlock = false;
			auto blockCollisionCheckFunctor =
				[ball, &hasBrokeOneBlock, &needInvertX, &needInvertY, this](auto object)
				{
					if (auto block = std::dynamic_pointer_cast<IBlockObject> (object))
					{
						if ((!hasBrokeOneBlock) && (block->CheckCollision(ball.get())))
						{
							hasBrokeOneBlock = true;
							sf::Vector2f ballPosition = ball->GetPosition();
							sf::FloatRect blockRect = block->GetRect();

							if (block->IsBallReboundable())
							{
								GetBallInverse(ballPosition, blockRect, needInvertX, needInvertY);
							}
						}
					}
				};

			for (auto block : gameObjects)
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

		if (breakableBlocksCount <= 0)
		{
			Application::GetInstance().GetGame()->LoadNextLevel();
		};
	}

	void PlayingState::Init()
	{
		auto self = weak_from_this();
		if (auto ball = std::dynamic_pointer_cast<IObservable>(gameObjects[1]); ball)
		{
			ball->AddObserver(self);
		}
		for (auto& bonusFactory : bonusFactories)
		{
			if (auto oneHitFactory = dynamic_cast<OneHitBlockBonusFactory*>(bonusFactory.second.get()))
			{
				oneHitFactory->SetObserver(self);
			}
		}
		scoreImage->SetPosition({ GameWorld::GetWorld()->ScreenSize().x, 0.f}, RelativePosition::TopRight);
		scoreImage->SetScoreCounter(Application::GetInstance().GetGame()->GetScoreCounter());
		Application::GetInstance().GetGame()->GetScoreCounter()->Reset();
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
			game->WinGame();
		}
		else
		{
			for (auto& object : gameObjects)
			{
				object->Reset();
			}
			ClearGameObjects<IBlockObject>();
			ClearGameObjects<IBonusObject>();
			CreateBlocks();
			ResetSessionDelay();
			++nextLevel;
		}
	}

	void PlayingState::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto block = std::dynamic_pointer_cast<IBlockObject>(observable); block)
		{
			if (auto bonus = BonusFactory::TryToCreateRandomBonus(bonusFactories, block->GetPosition()))
			{
				gameObjects.emplace_back(bonus);
				bonus->AddObserver(weak_from_this());
			}
			--breakableBlocksCount;
			objectsToDestroy.push_back(block);
		}
		else if (auto ball = std::dynamic_pointer_cast<IBallObject>(observable); ball)
		{
			if (ball->GetPosition().y > gameObjects.front()->GetRect().top)
			{
				auto game = Application::GetInstance().GetGame();
				game->LooseGame();
			}
		}
		else if (auto bonus = std::dynamic_pointer_cast<IBonusObject>(observable); bonus)
		{
			if (bonus->IsActivated())
			{
				for (auto& object : gameObjects)
				{
					bonus->Apply(object);
				}
				if (auto oneHitBonus = std::dynamic_pointer_cast<OneHitBlockBonus>(bonus))
				{
					CountBreakableBlocks();
				}
			}
			if (bonus->IsToBeDestroyed())
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
			stateSave->currentScore = Application::GetInstance().GetGame()->GetScoreCounter()->GetCurrentScore();
			stateSave->nextLevel = nextLevel;
			stateSave->blockTypes.clear();
			stateSave->blockSaves.clear();
			for (auto& object : gameObjects)
			{
				if (auto block = std::dynamic_pointer_cast<IBlockObject>(object))
				{
					stateSave->blockTypes.push_back(block->GetBlockType());
					stateSave->blockSaves.push_back(std::dynamic_pointer_cast<BlockSave>(block->SaveState()));
				}
				if (auto bonus = std::dynamic_pointer_cast<IBonusObject>(object))
				{
					stateSave->bonusTypes.push_back(bonus->GetType());
					stateSave->bonusSaves.push_back(std::dynamic_pointer_cast<BonusSave>(bonus->SaveState()));
				}
				if (auto platform = std::dynamic_pointer_cast<IPlatformObject>(object))
				{
					stateSave->platform = std::dynamic_pointer_cast<GameObjectSave>(platform->SaveState());
				}
				if (auto ball = std::dynamic_pointer_cast<IBallObject>(object))
				{
					stateSave->ball = std::dynamic_pointer_cast<BallSave>(ball->SaveState());
				}
			}
		}
	}

	void PlayingState::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto stateSave = std::dynamic_pointer_cast<PlayingStateSave>(save))
		{
			Application::GetInstance().GetGame()->GetScoreCounter()->SetScore(stateSave->currentScore);
			nextLevel = stateSave->nextLevel;
			ClearGameObjects<IBlockObject>();
			ClearGameObjects<IBonusObject>();
			for (auto& object : gameObjects)
			{
				if (std::dynamic_pointer_cast<IPlatformObject>(object))
				{
					object->LoadState(stateSave->platform);
				}
				else if (std::dynamic_pointer_cast<IBallObject>(object))
				{
					object->LoadState(stateSave->ball);
				}
			}
			CreateBlocks(stateSave);
			for (size_t i = 0; i < stateSave->bonusTypes.size(); ++i)
			{
				gameObjects.push_back(CreateBonusByType(bonusFactories, stateSave->bonusTypes[i]));
				std::dynamic_pointer_cast<IObservable>(gameObjects.back())->AddObserver(weak_from_this());
				gameObjects.back()->LoadState(stateSave->bonusSaves[i]);
			}
			std::for_each(gameObjects.begin(), gameObjects.end(), [](auto& object) { object->Update(0.f); });
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
		auto scoreCounter = Application::GetInstance().GetGame()->GetScoreCounter();
		auto addBlock = [this, self, scoreCounter](auto& block) {
			gameObjects.push_back(block);
			block->AddObserver(scoreCounter);
			block->AddObserver(self);
			};
		if (stateSave)
		{
			for (int i = 0; i < stateSave->blockSaves.size(); ++i)
			{
				auto newBlock = blockFactories.at(stateSave->blockTypes[i])->CreateBlock();
				addBlock(newBlock);
				newBlock->LoadState(stateSave->blockSaves[i]);
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
				auto newBlock = blockFactories.at(block.second)->CreateBlock(position);
				addBlock(newBlock);
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

	void PlayingState::CountBreakableBlocks()
	{
		breakableBlocksCount = 0;
		for (auto& object : gameObjects)
		{
			if (auto block = std::dynamic_pointer_cast<IBlockObject> (object))
			{
				breakableBlocksCount += block->IsBreakable() ? 1 : 0;
			}
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
			bonusSaves.push_back(std::make_shared<BonusSave>());
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