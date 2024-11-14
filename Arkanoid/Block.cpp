#include "Block.h"
#include "Application.h"
#include "Utility.h"

namespace Arkanoid
{
	Block::Block(const sf::Vector2f& position, const sf::Color color) : GameObject("platform.png")
	{
		sprite.setColor(color);
		Settings* settings = Application::GetSettings();
		SetScaleBySize(sprite, settings->blockSize);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sprite.setPosition(position);
	}

	bool Block::GetCollision(Collidable* object) const
	{
		auto gameObject = dynamic_cast<GameObject*>(object);
		assert(gameObject);
		return GetRect().intersects(gameObject->GetRect());
	}

	bool Block::IsBroken()
	{
		return HitCount <= 0;
	}

	void Block::OnHit()
	{
		--HitCount;
	}
}