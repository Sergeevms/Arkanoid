#pragma once
#include "GameObject.h"
#include "IDelayedAction.h"
#include "IObserver.h"

namespace Arkanoid
{
	enum class BlockType
	{
		Simple,
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

		static BlockType GetBlockType(const Block* block);

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
		virtual void LoadFromFile(std::ifstream& ifstream) override;
		BlockType GetBlockType();
	private:
		friend class Block;
		BlockType blockType;
		int hitCount;
	};

	class UnbreakbleBlock : public Block
	{
	public:
		UnbreakbleBlock(const sf::Vector2f& position);
		virtual void SaveState(std::shared_ptr<ISave> save) const;
	protected:
		virtual void OnHit() override;
	};

	class SmoothDestroyableBlock : public Block, public IDelayedAction
	{
	public:
		SmoothDestroyableBlock(const sf::Vector2f& position);
		virtual bool GetCollision(Collidable* object) const override;
		virtual void Update(const float deltaTime) override;
	protected:
		virtual void OnHit() override;
		virtual void FinalAction() override;
		virtual void UpdateAction(float deltaTime) override;
	};

	class MultiHitBlock : public Block
	{
	public:
		MultiHitBlock(const sf::Vector2f& position);
		virtual int GetScore() const override;
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
	};

	std::shared_ptr<Block> CreateRandomBlock(sf::Vector2f position);
}

