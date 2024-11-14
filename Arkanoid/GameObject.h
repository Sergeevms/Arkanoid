#pragma once
#include "SFML/Graphics.hpp"

namespace Arkanoid
{
	class GameObject
	{
	public:
		GameObject(const std::string textureName);
		virtual ~GameObject() = default;
		virtual void Draw(sf::RenderWindow& window) const;
		virtual void Update(const float deltaTime) = 0;
		sf::FloatRect GetRect() const;
		sf::Vector2f GetPosition() const;
		sf::Vector2f HalfSize() const;
	protected:
		sf::Texture texture;
		sf::Sprite sprite;
	};
}

