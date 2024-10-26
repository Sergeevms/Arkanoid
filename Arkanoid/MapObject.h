#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"

namespace SnakeGame
{

	class MapObject
	{
	public:
		MapObject(const sf::Vector2i& mapCell, const sf::Texture& texture);
		virtual ~MapObject() = default;
		virtual void SetColor(sf::Color newColor);
		virtual void Draw(sf::RenderWindow&) const;
		sf::Vector2i GetCellPosition() const;
	protected:
		sf::Vector2i mapCoordinates;
		sf::Vector2f screenCoordinates;
		sf::Sprite sprite;
	};

	//returns true if object colliding leading to game over, false otherwise
	bool IsCollisionOveringGame(MapObject*);
}