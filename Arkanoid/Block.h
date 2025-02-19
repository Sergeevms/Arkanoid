#pragma once
#include "GameObject.h"
#include "IDelayedAction.h"
#include "IObserver.h"

namespace Arkanoid
{
	enum class BlockType
	{
		Simple,
		SmoothDestroyable,
		Unbreackble,
		MultiHit,
		Glass
	};

	class Block : public GameObject, public IObservable
	{
	public:
		Block(const sf::Vector2f& position);
		virtual void Update(const float deltaTime) override {} ;
		virtual bool GetCollision(Collidable* object) const override;
		virtual bool IsBroken();
		virtual bool IsBallReboundable();
		virtual int GetScore() const;
		virtual BlockType GetBlockType() const { return BlockType::Simple; };

		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save);
	protected:
		virtual void OnHit() override;
		int hitCount = 1;
	};

	class BlockSave : public GameObjectSave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const override;
		virtual void LoadFromFile(std::ifstream& istream) override;
	private:
		friend class Block;
		int hitCount;
	};

	class UnbreakbleBlock : public Block
	{
	public:
		UnbreakbleBlock(const sf::Vector2f& position);
		virtual BlockType GetBlockType() const override { return BlockType::Unbreackble; };
	protected:
		virtual void OnHit() override;
	};

	class SmoothDestroyableBlockSave : public BlockSave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const override;
		virtual void LoadFromFile(std::ifstream& istream) override;
	private:
		friend class SmoothDestroyableBlock;
		float currentTime;
		float delayDuration;
	};

	class SmoothDestroyableBlock : public Block, public IDelayedAction
	{
	public:
		SmoothDestroyableBlock(const sf::Vector2f& position);
		virtual bool GetCollision(Collidable* object) const override;
		virtual void Update(const float deltaTime) override;
		virtual int GetScore() const override;
		virtual BlockType GetBlockType() const override { return BlockType::SmoothDestroyable; };

		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save);
	protected:
		virtual void OnHit() override;
		virtual void FinalAction() override;
		virtual void UpdateAction(float deltaTime) override;
		void UpdateOpacity();
	};

	class MultiHitBlock : public Block
	{
	public:
		MultiHitBlock(const sf::Vector2f& position);
		virtual int GetScore() const override;
		virtual BlockType GetBlockType() const override { return BlockType::MultiHit; };
	protected:
		virtual void OnHit() override;
	};

	class GlassBlock : public Block
	{
	public:
		GlassBlock(const sf::Vector2f& position);
		virtual int GetScore() const override;
		virtual bool CheckCollision(Collidable* object) override;
		virtual bool IsBallReboundable() override;
		virtual BlockType GetBlockType() const override { return BlockType::Glass; };
	};

	std::shared_ptr<Block> CreateRandomBlock(const sf::Vector2f position);
	std::shared_ptr<BlockSave> CreateEmptySaveByBlockType(const BlockType blockType);
}

