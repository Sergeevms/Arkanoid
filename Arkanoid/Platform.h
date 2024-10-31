#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	enum class Direction;

	class Platform
	{
	public:
		Platform();
		void SetMovingDirection(const Direction direction);
		void Update(const float deltaTime);
		void Draw(sf::RenderWindow& window) const;
		sf::FloatRect GetRect() const;
	private:
		sf::Vector2f HalfSize() const;
		sf::Sprite sprite;
		sf::Texture texture;
		Direction direction;
	};
}

