#include "Block.h"
#include "Application.h"
#include "Utility.h"

namespace Arkanoid
{
	Block::Block(const sf::Vector2f& position) : GameObject("platform.png")
	{
		sprite.setColor(sf::Color::Red);
		Settings* settings = Application::GetSettings();
		SetScaleBySize(sprite, settings->blockSize);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sprite.setPosition(position);
	}
}