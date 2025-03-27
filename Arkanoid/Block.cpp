#include "Block.h"
#include "Utility.h"
#include "GameWorld.h"

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
		auto gameObject = dynamic_cast<IGameObject*>(object);
		assert(gameObject);
		sf::FloatRect objectRect = gameObject->GetRect();
		return GetRect().intersects(gameObject->GetRect());
	}

	bool Block::IsBroken() const
	{
		return hitCount <= 0;
	}

	bool Block::IsBreakable() const
	{
		return true;
	}

	bool Block::IsBallReboundable() const
	{
		return true;
	}

	int Block::GetScore() const
	{
		return GameWorld::GetWorld()->blockScore[GetBlockType()];
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
		}
	}

	void Block::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto blockSave = std::dynamic_pointer_cast<BlockSave>(save))
		{
			GameObject::LoadState(save);
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

	bool UnbreakbleBlock::IsBreakable() const
	{
		return false;
	}

	void UnbreakbleBlock::OnHit()
	{
		
	}

	std::shared_ptr<Block> CreateRandomBlock(const sf::Vector2f position)
	{
		GameWorld* world = GameWorld::GetWorld();
		BlockType newBlockType = world->availiableBlockTypes[rand() % world->availiableBlockTypes.size()];
		switch (newBlockType)
		{
		case Arkanoid::BlockType::SmoothDestroyable:
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
			return std::make_shared<Block>(position);
			break;
		}
		}
	}

	std::shared_ptr<BlockSave> CreateEmptySaveByBlockType(const BlockType blockType)
	{
		switch (blockType)
		{
		case BlockType::SmoothDestroyable:
			return std::make_shared<SmoothDestroyableBlockSave>();
			break;
		default:
			return std::make_shared<BlockSave>();
			break;
		}
	}

	SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::SmoothDestroyable]);
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

	std::shared_ptr<ISave> SmoothDestroyableBlock::SaveState() const
	{
		auto blockSave = std::make_shared<SmoothDestroyableBlockSave>();
		SaveState(blockSave);
		return blockSave;
	}

	void SmoothDestroyableBlock::SaveState(std::shared_ptr<ISave> save) const
	{
		if (auto blockSave = std::dynamic_pointer_cast<SmoothDestroyableBlockSave>(save))
		{
			Block::SaveState(blockSave);
			blockSave->currentTime = currentTime;
			blockSave->delayDuration = delayDuration;
		}
	}

	void SmoothDestroyableBlock::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto blockSave = std::dynamic_pointer_cast<SmoothDestroyableBlockSave>(save))
		{
			Block::LoadState(blockSave);
			currentTime = blockSave->currentTime;
			delayDuration = blockSave->delayDuration;
			UpdateOpacity();
		}
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
		UpdateOpacity();
	}

	void SmoothDestroyableBlock::UpdateOpacity()
	{
		if (currentTime > 0.f)
		{
			ChangeSpriteOpacity(sprite, static_cast<sf::Uint8> (255 * currentTime / delayDuration));
		}
		else
		{
			ChangeSpriteOpacity(sprite, 255);
		}
	}

	MultiHitBlock::MultiHitBlock(const sf::Vector2f& position) : Block(position)
	{
		hitCount = GameWorld::GetWorld()->multiHitBlockCount;
		sprite.setColor(GameWorld::GetWorld()->blockColors[BlockType::MultiHit]);
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

	bool GlassBlock::CheckCollision(Collidable* object)
	{
		if (GetCollision(object))
		{
			OnHit();
			return true;
		}
		return false;
	}

	bool GlassBlock::IsBallReboundable() const
	{
		return false;
	}

	void BlockSave::SaveToFile(std::ofstream& ostream) const
	{
		GameObjectSave::SaveToFile(ostream);
		ostream << hitCount << " ";
	}

	void BlockSave::LoadFromFile(std::ifstream& istream)
	{
		GameObjectSave::LoadFromFile(istream);
		istream >> hitCount;
	}

	void SmoothDestroyableBlockSave::SaveToFile(std::ofstream& ostream) const
	{
		BlockSave::SaveToFile(ostream);
		ostream << currentTime << " " << delayDuration << " ";
	}

	void SmoothDestroyableBlockSave::LoadFromFile(std::ifstream& istream)
	{
		BlockSave::LoadFromFile(istream);
		istream >> currentTime >> delayDuration;
	}
}