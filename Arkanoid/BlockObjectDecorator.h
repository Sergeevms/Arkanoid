#pragma once
#pragma warning( push )
#pragma warning( disable : 4250)

#include "GameObjectDecorator.h"
#include "IBlockObject.h"

namespace Arkanoid
{
	class BlockObjectDecorator : public virtual GameObjectDecorator, public virtual IBlockObject
	{
	public:
		BlockObjectDecorator(std::shared_ptr<IBlockObject> object);
		virtual bool IsBroken() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBroken(); };
		virtual bool IsBreakable() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBreakable(); };
		virtual bool IsBallReboundable() const { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBallReboundable(); };
		virtual int GetScore() const { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->GetScore(); };
		virtual BlockType GetBlockType() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->GetBlockType(); };
	};

	class OneHitBlockDecorator : public BlockObjectDecorator, public SpriteChangedDecorator
	{
	public:
		OneHitBlockDecorator(std::shared_ptr<IBlockObject> object);
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void OnHit() override;
		virtual bool IsBroken() const override;
		virtual bool IsBreakable() const override;
		virtual void Update(const float deltaTime) override;
		virtual void AddObserver(std::weak_ptr<IObserver> observer) override;
		virtual std::shared_ptr<IObservable> GetObservablePtr() override;
	protected:
		bool isBroken = false;
	};
}

#pragma warning( pop )