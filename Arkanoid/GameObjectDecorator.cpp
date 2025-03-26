#include "GameObjectDecorator.h"
#include "Utility.h"
#include "Platform.h"
#include "Ball.h"
#include "Bonus.h"
#include "GameWorld.h"

namespace Arkanoid
{
	SpriteChangedDecorator::SpriteChangedDecorator(std::shared_ptr<IGameObject> object, const std::string textureName) : BaseGameObjectDecorator(object)
	{
		LoadTexture(textureName, texture);
		sprite.setTexture(texture);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
	}

	void SpriteChangedDecorator::Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	void SpriteChangedDecorator::Update(const float deltaTime)
	{
		wrappedObject->Update(deltaTime);
		sprite.setPosition(wrappedObject->GetPosition());
		sprite.setColor(wrappedObject->GetColor());
		sf::Rect objectRect = wrappedObject->GetRect();
		SetScaleBySize(sprite, { objectRect.width, objectRect.height });
	}

	sf::FloatRect SpriteChangedDecorator::GetRect() const
	{
		return sprite.getGlobalBounds();
	}

	sf::Vector2f SpriteChangedDecorator::HalfSize() const
	{
		sf::Vector2f size{ sprite.getGlobalBounds().width, sprite.getGlobalBounds().height };
		return size / 2.f;
	}

	PlatformSizeDecorator::PlatformSizeDecorator(std::shared_ptr<IGameObject> object, const sf::Vector2f sizeScale) :
		BaseGameObjectDecorator(object), PlatformDecorator(object), SpriteChangedDecorator(object, "Platform.png"), scale(sizeScale)
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
			return Platform::CheckCollisionWithCircle(GetRect(), ball->GetPosition(), GameWorld::GetWorld()->ballDiameter / 2.f);
		}
		else if (auto bonus = dynamic_cast<IBonusObject*>(object))
		{
			return (!bonus->IsActivated()) && Platform::CheckCollisionWithCircle(GetRect(), bonus->GetPosition(), GameWorld::GetWorld()->bonusSize / 2.f);
		}
		else
		{
			return false;
		}
	}

	std::shared_ptr<IGameObject> BaseGameObjectDecorator::RemoveDecorator(std::weak_ptr<IGameObject> objectPtr, bool & decoratorRemoved)
	{
		auto object = objectPtr.lock();
		if (object == shared_from_this())
		{
			decoratorRemoved = true;
			return wrappedObject;
		}
		else
		{
			if (auto wrapped = std::dynamic_pointer_cast<BaseGameObjectDecorator> (wrappedObject))
			{
				wrappedObject = wrapped->RemoveDecorator(objectPtr, decoratorRemoved);
			}
			return shared_from_this();
		}
	}

	BallDecorator::BallDecorator(std::shared_ptr<IGameObject> object) : BaseGameObjectDecorator(object)
	{
		assert(std::dynamic_pointer_cast<IBallObject>(object));
	}

	PlatformDecorator::PlatformDecorator(std::shared_ptr<IGameObject> object) : BaseGameObjectDecorator(object)
	{
		assert(std::dynamic_pointer_cast<IPlatformObject>(object));
	}

	BlockDecorator::BlockDecorator(std::shared_ptr<IGameObject> object) : BaseGameObjectDecorator(object)
	{
		assert(std::dynamic_pointer_cast<IBlockObject>(object));
	}

	BallSpeedDecorator::BallSpeedDecorator(std::shared_ptr<IGameObject> object, float updateTimeCoefficient)
		: BaseGameObjectDecorator(object), BallDecorator(object), SpriteChangedDecorator(object, "Ball.png"), coefficient(updateTimeCoefficient)
	{
	}

	void BallSpeedDecorator::Update(const float deltaTime)
	{		
		SpriteChangedDecorator::Update(deltaTime * coefficient);
		sprite.setColor(GameWorld::GetWorld()->acceleratedBallColor);
	}

	OneHitBlockDecorator::OneHitBlockDecorator(std::shared_ptr<IGameObject> object)
		: BaseGameObjectDecorator(object), BlockDecorator(object), SpriteChangedDecorator(object, "Platform.png")
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

	std::shared_ptr<IObservable> OneHitBlockDecorator::GetObservablePtr()
	{
		return std::dynamic_pointer_cast<IObservable>(shared_from_this());
	}
}