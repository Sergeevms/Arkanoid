#include "Block.h"
#include "Application.h"
#include "Utility.h"

namespace Arkanoid
{
	Block::Block(const sf::Vector2f& position) : GameObject("platform.png")
	{
		GameWorld* world = GameWorld::GetWorld();
		sprite.setColor(world->blockColors[BlockType::Simple]);
		SetScaleBySize(sprite, world->blockSize);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sprite.setPosition(position);
	}

	bool Block::GetCollision(Collidable* object) const
	{
		auto gameObject = dynamic_cast<GameObject*>(object);
		assert(gameObject);
		sf::FloatRect objectRect = gameObject->GetRect();
		return GetRect().intersects(gameObject->GetRect());
	}

	bool Block::IsBroken()
	{
		return hitCount <= 0;
	}

	bool Block::IsBallReboundable()
	{
		return true;
	}

	int Block::GetScore() const
	{
		return GameWorld::GetWorld()->blockScore[BlockType::Simple];
	}

	std::shared_ptr<ISave> Block::SaveState() const
	{
		auto blockSave = std::make_shared<BlockSave>();
		SaveState(blockSave);
		return blockSave;
	}

	void Block::SaveState(std::shared_ptr<ISave> save) const
	{
		if (auto blockSave = std::dynamic_pointer_cast<BlockSave>(save))
		{
			GameObject::SaveState(blockSave);
			blockSave->hitCount = hitCount;
			blockSave->blockType = GetBlockType(this);
		}
	}

	void Block::LoadState(const std::shared_ptr<ISave> save)
	{
		GameObject::LoadState(save);
		if (auto blockSave = std::dynamic_pointer_cast<BlockSave>(save))
		{
			hitCount = blockSave->hitCount;
		}
	}

	void Block::OnHit()
	{
		--hitCount;
		Emit();
	}

	UnbreakbleBlock::UnbreakbleBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::Unbreackble]);
	}

	void UnbreakbleBlock::SaveState(std::shared_ptr<ISave> save) const
	{
		auto blockSave = std::dynamic_pointer_cast<BlockSave>(save);
		Block::SaveState(blockSave);
	}

	void UnbreakbleBlock::OnHit()
	{
		
	}

	BlockType Block::GetBlockType(const Block* block)
	{
		BlockType blockType = BlockType::Simple;
		if (dynamic_cast<const UnbreakbleBlock*>(block))
		{
			blockType = BlockType::Unbreackble;
		}
		else if (dynamic_cast<const MultiHitBlock*>(block))
		{
			blockType = BlockType::MultiHit;
		}
		else if (dynamic_cast<const GlassBlock*>(block))
		{
			blockType = BlockType::Glass;
		}
		return blockType;
	}

	std::shared_ptr<Block> CreateRandomBlock(sf::Vector2f position)
	{
		GameWorld* world = GameWorld::GetWorld();
		BlockType newBlockType = world->availiableBlockTypes[rand() % world->availiableBlockTypes.size()];
		switch (newBlockType)
		{
		case Arkanoid::BlockType::Simple:
		{
			return std::make_shared<SmoothDestroyableBlock>(position);
			break;
		}
		case Arkanoid::BlockType::Unbreackble:
		{
			return std::make_shared<UnbreakbleBlock>(position);
			break;
		}
		case Arkanoid::BlockType::MultiHit:
		{
			return std::make_shared<MultiHitBlock>(position);
			break;
		}
		case Arkanoid::BlockType::Glass:
		{
			return std::make_shared<GlassBlock>(position);
			break;
		}
		default:
		{
			return nullptr;
			break;
		}
		}
	}

	SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::Simple]);
	}

	bool SmoothDestroyableBlock::GetCollision(Collidable* object) const
	{
		if (currentTime > 0.f)
		{
			return false;
		}
		return Block::GetCollision(object);
	}

	void SmoothDestroyableBlock::Update(const float deltaTime)
	{
		UpdateTimer(deltaTime);
	}

	void SmoothDestroyableBlock::OnHit()
	{
		StartTimer(GameWorld::GetWorld()->smoothBlockDestroyTime);
	}

	void SmoothDestroyableBlock::FinalAction()
	{
		--hitCount;
		Emit();
	}

	void SmoothDestroyableBlock::UpdateAction(float deltaTime)
	{
		ChangeSpriteOpacity(sprite, static_cast<sf::Uint8> (255 * currentTime / delayDuration));
	}

	MultiHitBlock::MultiHitBlock(const sf::Vector2f& position) : Block(position)
	{
		hitCount = GameWorld::GetWorld()->multiHitBlockCount;
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::MultiHit]);
	}

	int MultiHitBlock::GetScore() const
	{
		return GameWorld::GetWorld()->blockScore[BlockType::MultiHit];
	}

	void MultiHitBlock::OnHit()
	{
		--hitCount;
		ChangeSpriteOpacity(sprite, 255 * hitCount / GameWorld::GetWorld()->multiHitBlockCount);
		if (IsBroken())
		{
			Emit();
		}
	}

	GlassBlock::GlassBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::Glass]);
	}

	int GlassBlock::GetScore() const
	{
		return GameWorld::GetWorld()->blockScore[BlockType::Glass];
	}

	bool GlassBlock::CheckCollision(Collidable* object)
	{
		if (GetCollision(object))
		{
			OnHit();
			return true;
		}
		return false;
	}

	bool GlassBlock::IsBallReboundable()
	{
		return false;
	}

	void BlockSave::SaveToFile(std::ofstream& ostream) const
	{
		GameObjectSave::SaveToFile(ostream);
		ostream << static_cast<int>(blockType) << " ";
		ostream << hitCount << " ";
	}

	void BlockSave::LoadFromFile(std::ifstream& ifstream)
	{
		GameObjectSave::LoadFromFile(ifstream);
		int tBlockType = 0;
		ifstream >> tBlockType >> hitCount;
		blockType = static_cast<BlockType>(tBlockType);
	}
	BlockType BlockSave::GetBlockType()
	{
		return blockType;
	}
}