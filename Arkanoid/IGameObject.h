#pragma once
#include <SFML/Graphics.hpp>
#include "Collidable.h"
#include "IObserver.h"
#include "ISaveable.h"

namespace Arkanoid
{
	class IGameObject : public Collidable, public IObservable, public ISaveable
	{
	public:
		virtual ~IGameObject() {};
		virtual void Draw(sf::RenderWindow& window) const = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual sf::FloatRect GetRect() const = 0;
		virtual sf::Vector2f GetPosition() const = 0;
		virtual void SetPosition(const sf::Vector2f newPosition) = 0;
		virtual sf::Vector2f HalfSize() const = 0;
		virtual void Reset() = 0;
		virtual sf::Color GetColor() = 0;
	};
}
