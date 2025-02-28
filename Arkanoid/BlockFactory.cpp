#include "BlockFactory.h"
#include "Block.h"

namespace Arkanoid
{
	std::shared_ptr<Block> BlockFactory::CreateBlock(const sf::Vector2f position)
	{
		++createdBreakableBlocks;
		return std::make_shared<Block>(position);
	}

	int BlockFactory::GetBreakableBlockCount() const
	{
		return createdBreakableBlocks;
	}

	void BlockFactory::ClearBreakableCounter()
	{
		createdBreakableBlocks = 0;
	}

	std::shared_ptr<Block> SmoothDestroyableBlockFactory::CreateBlock(const sf::Vector2f position)
	{
		++createdBreakableBlocks;
		return std::make_shared<SmoothDestroyableBlock>(position);
	}

	std::shared_ptr<Block> MultipleHitBlockFactory::CreateBlock(const sf::Vector2f position)
	{
		++createdBreakableBlocks;
		return std::make_shared<MultiHitBlock>(position);
	}

	std::shared_ptr<Block> GlassBlockFactory::CreateBlock(const sf::Vector2f position)
	{
		++createdBreakableBlocks;
		return std::make_shared<GlassBlock>(position);
	}

	std::shared_ptr<Block> UnbreakableBlockFactory::CreateBlock(const sf::Vector2f position)
	{
		return std::make_shared<UnbreakbleBlock>(position);
	}

	std::unique_ptr<BlockFactory> BlockFactory::CreateFactory(BlockType blockType)
	{
		switch (blockType)
		{
		case Arkanoid::BlockType::Simple:
			return std::make_unique<BlockFactory>();
			break;
		case Arkanoid::BlockType::SmoothDestroyable:
			return std::make_unique<SmoothDestroyableBlockFactory>();
			break;
		case Arkanoid::BlockType::Unbreackble:
			return std::make_unique<UnbreakableBlockFactory>();
			break;
		case Arkanoid::BlockType::MultiHit:
			return std::make_unique<MultipleHitBlockFactory>();
			break;
		case Arkanoid::BlockType::Glass:
			return std::make_unique<GlassBlockFactory>();
			break;
		default:
			return nullptr;
			break;
		}
	}
}