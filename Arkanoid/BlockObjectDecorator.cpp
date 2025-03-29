#include "BlockObjectDecorator.h"
#include "GameWorld.h"

namespace Arkanoid
{
	BlockObjectDecorator::BlockObjectDecorator(std::shared_ptr<IBlockObject> object) : GameObjectDecorator(object)
	{
	}

	OneHitBlockDecorator::OneHitBlockDecorator(std::shared_ptr<IBlockObject> object)
		: GameObjectDecorator(object), BlockObjectDecorator(object), SpriteChangedDecorator(object, "Platform.png")
	{

	}

	void OneHitBlockDecorator::Draw(sf::RenderWindow& window) const
	{
		wrappedObject->Draw(window);
		window.draw(sprite);
	}

	void OneHitBlockDecorator::OnHit()
	{
		isBroken = true;
		Emit();
	}

	bool OneHitBlockDecorator::IsBroken() const
	{
		return isBroken;
	}

	bool OneHitBlockDecorator::IsBreakable() const
	{
		return true;
	}

	void OneHitBlockDecorator::Update(const float deltaTime)
	{
		SpriteChangedDecorator::Update(deltaTime);
		sprite.setColor(GameWorld::GetWorld()->oneHitBlockColor);
	}

	void OneHitBlockDecorator::AddObserver(std::weak_ptr<IObserver> observer)
	{
		observers.push_back(observer);
	}
}