#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include "IDelayedAction.h"

namespace Arkanoid
{
	class Block : public GameObject, public Collidable
	{
	public:
		Block(const sf::Vector2f& position);
		virtual void Update(const float deltaTime) override {} ;
		virtual bool GetCollision(Collidable* object) const override;
		virtual bool IsBroken();
		virtual bool IsBallReboundable();
	protected:
		virtual void OnHit() override;
		int HitCount = 1;
	};

	enum class BlockType
	{
		Simple,
		Unbreackble,
		MultiHit,
		Glass
	};

	class UnbreakbleBlock : public Block
	{
	public:
		UnbreakbleBlock(const sf::Vector2f& position);
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
	protected:
		virtual void OnHit() override;
	};

	class GlassBlock : public Block
	{
	public:
		GlassBlock(const sf::Vector2f& position);
		virtual bool CheckCollision(Collidable* object) override;
		virtual bool IsBallReboundable() override;
	};

	std::shared_ptr<Block> CreateRandomBlock(sf::Vector2f position);
}

