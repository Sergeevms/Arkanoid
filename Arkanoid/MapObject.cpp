#include "MapObject.h"
#include "Settings.h"
#include "Wall.h"
#include "Snake.h"

namespace SnakeGame
{
	MapObject::MapObject(const sf::Vector2i& mapCell, const sf::Texture& texture): 
		sprite(texture), mapCoordinates(mapCell)
	{
		float tileSize = Settings::GetSettings()->tileSize;
		SetScaleBySize(sprite, { tileSize, tileSize });
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sf::Vector2f mapCellF(mapCell);
		screenCoordinates = (mapCellF + relativePositions.at(RelativePosition::Center)) * tileSize;
		sprite.setPosition(screenCoordinates);
	}

	void MapObject::SetColor(sf::Color newColor)
	{
		sprite.setColor(newColor);
	}

	void MapObject::Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	sf::Vector2i MapObject::GetCellPosition() const
	{
		return mapCoordinates;
	}

	bool IsCollisionOveringGame(MapObject* object)
	{
		if (object != nullptr && (dynamic_cast<Wall*>(object) != nullptr || dynamic_cast<SnakeNode*> (object) != nullptr))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}