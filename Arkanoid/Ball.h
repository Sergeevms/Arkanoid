#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Platform;

	class Ball
	{
	public:
		Ball();
		void Update(const float deltaTime);
		void Draw(sf::RenderWindow& window);
		void CheckPlatformCollision(const Platform* platform);
		void Reset();
	private:
		sf::Vector2f HalfSize() const;
		sf::Sprite sprite;
		sf::Texture texture;
		sf::Vector2f direction;
	};
}

