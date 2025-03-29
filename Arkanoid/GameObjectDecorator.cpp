#include "GameObjectDecorator.h"
#include "Utility.h"

namespace Arkanoid
{
	SpriteChangedDecorator::SpriteChangedDecorator(std::shared_ptr<IGameObject> object, const std::string textureName) : GameObjectDecorator(object)
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

	std::shared_ptr<IGameObject> GameObjectDecorator::RemoveDecorator(std::weak_ptr<IGameObject> objectPtr, bool & decoratorRemoved)
	{
		auto object = objectPtr.lock();
		if (object == shared_from_this())
		{
			decoratorRemoved = true;
			return wrappedObject;
		}
		else
		{
			if (auto wrapped = std::dynamic_pointer_cast<GameObjectDecorator> (wrappedObject))
			{
				wrappedObject = wrapped->RemoveDecorator(objectPtr, decoratorRemoved);
			}
			return std::dynamic_pointer_cast<IGameObject>(shared_from_this());
		}
	}
}