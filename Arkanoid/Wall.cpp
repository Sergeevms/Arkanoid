#include "Wall.h"

namespace SnakeGame
{
	Wall::Wall(const sf::Vector2i& mapCell, const sf::Texture& texture, const Direction direction) : MapObject(mapCell, texture)
	{		
		sprite.setRotation(directionToRotation.at(direction));
	}

	TemporaryWall::TemporaryWall(const sf::Vector2i& mapCell, const sf::Texture& texture, const Direction direction) : 
		Wall(mapCell, texture, direction)
	{
		sprite.setColor(sf::Color::Yellow);
	}

	void TemporaryWall::setOpacity(const int opacity)
	{
		sf::Color newColor = sprite.getColor();
		newColor.a = opacity;
		sprite.setColor(newColor);
	}
}