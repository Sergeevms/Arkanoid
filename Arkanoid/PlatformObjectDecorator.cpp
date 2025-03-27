#include "PlatformObjectDecorator.h"
#include "GameWorld.h"
#include "Utility.h"
#include "IBallObject.h"
#include "IBonusObject.h"

namespace Arkanoid
{
	PlatformObjectDecorator::PlatformObjectDecorator(std::shared_ptr<IPlatformObject> object) : GameObjectDecorator(object)
	{
		assert(std::dynamic_pointer_cast<IPlatformObject>(object));
	}

	PlatformSizeDecorator::PlatformSizeDecorator(std::shared_ptr<IPlatformObject> object, const sf::Vector2f sizeScale) :
		GameObjectDecorator(object), PlatformObjectDecorator(object), SpriteChangedDecorator(object, "Platform.png"), scale(sizeScale)
	{

	}

	void PlatformSizeDecorator::Update(const float deltaTime)
	{
		SpriteChangedDecorator::Update(deltaTime);
		sf::Rect objectRect = wrappedObject->GetRect();
		SetScaleBySize(sprite, { objectRect.width * scale.x, objectRect.height * scale.y });
		sf::Vector2f newPosition = GetPosition();
		newPosition.x = std::clamp(newPosition.x, HalfSize().x, GameWorld::GetWorld()->screenWidth - HalfSize().x);
		SetPosition(newPosition);
		wrappedObject->SetPosition(newPosition);
	}

	bool PlatformSizeDecorator::GetCollision(Collidable* object) const
	{
		if (auto ball = dynamic_cast<IBallObject*>(object))
		{
			return CheckRectCollisionWithCircle(GetRect(), ball->GetPosition(), GameWorld::GetWorld()->ballDiameter / 2.f);
		}
		else if (auto bonus = dynamic_cast<IBonusObject*>(object))
		{
			return (!bonus->IsActivated()) && CheckRectCollisionWithCircle(GetRect(), bonus->GetPosition(), GameWorld::GetWorld()->bonusSize / 2.f);
		}
		else
		{
			return false;
		}
	}
}