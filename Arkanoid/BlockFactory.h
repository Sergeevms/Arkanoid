#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Block;
	enum class BlockType;

	class BlockFactory
	{
	public:
		virtual ~BlockFactory() = default;
		virtual std::shared_ptr<Block> CreateBlock(const sf::Vector2f position = { 0.f, 0.f });
		int GetBreakableBlockCount() const;
		void ClearBreakableCounter();
		static std::unique_ptr<BlockFactory> CreateFactory(const BlockType);
	protected:
		int createdBreakableBlocks = 0;
	};	

	class SmoothDestroyableBlockFactory : public BlockFactory
	{
	public:
		virtual std::shared_ptr<Block> CreateBlock(const sf::Vector2f position = { 0.f, 0.f }) override;
	};

	class MultipleHitBlockFactory : public BlockFactory
	{
	public:
		virtual std::shared_ptr<Block> CreateBlock(const sf::Vector2f position = { 0.f, 0.f }) override;
	};

	class GlassBlockFactory : public BlockFactory
	{
	public:
		virtual std::shared_ptr<Block> CreateBlock(const sf::Vector2f position = { 0.f, 0.f }) override;
	};
	
	class UnbreakableBlockFactory : public BlockFactory
	{
	public:
		virtual std::shared_ptr<Block> CreateBlock(const sf::Vector2f position = { 0.f, 0.f }) override;
	};
}
