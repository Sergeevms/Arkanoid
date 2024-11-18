#include "BlockFactory.h"
#include "Block.h"

namespace Arkanoid
{
	int BlockFactory::GetBreakableBlockCount() const
	{
		return createdBreakableBlocks;
	}

	void BlockFactory::ClearBreakableCounter()
	{
		createdBreakableBlocks = 0;
	}

	std::shared_ptr<Block> SimpleBlockFactory::CreateBlock(const sf::Vector2f position)
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

}