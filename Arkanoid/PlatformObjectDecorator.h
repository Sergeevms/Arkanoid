#pragma once
#pragma warning( push )
#pragma warning( disable : 4250)

#include "GameObjectDecorator.h"
#include "IPlatformObject.h"
namespace Arkanoid
{
	class PlatformObjectDecorator : public virtual GameObjectDecorator, public virtual IPlatformObject
	{
	public:
		PlatformObjectDecorator(std::shared_ptr<IPlatformObject> object);
		virtual void SetMovingDirection(const Direction direction) override { std::dynamic_pointer_cast<IPlatformObject>(wrappedObject)->SetMovingDirection(direction); };
	};

	class PlatformSizeDecorator : public PlatformObjectDecorator, public SpriteChangedDecorator
	{
	public:
		PlatformSizeDecorator(std::shared_ptr<IPlatformObject> object, const sf::Vector2f scale);
		virtual void Update(const float deltaTime) override;
		bool GetCollision(Collidable* object) const override;
	protected:
		sf::Vector2f scale;
	};
}

#pragma warning( pop )