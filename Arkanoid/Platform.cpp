#include "Platform.h"
#include "Utility.h"
#include "Application.h"

namespace Arkanoid
{
	Platform::Platform() : GameObject("platform.png")
	{
		Settings* settings = Application::GetSettings();
		SetScaleBySize(sprite, settings->platformSize);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		Reset();
	}

	void Platform::SetMovingDirection(const Direction direction)
	{
		Platform::direction = direction;
	}

	void Platform::Update(const float deltaTime)
	{
		sf::Vector2f directionVector(0.f, 0.f);
		switch (direction)
		{
		case (Direction::Left):
		{
			directionVector.x = -1.f;
			break;
		}
		case (Direction::Rigth):
		{
			directionVector.x = 1.f;
			break;
		}
		default:
			break;
		}
		Settings* settings = Application::GetSettings();
		sf::Vector2f newPosition = sprite.getPosition() + directionVector * settings->platformSpeed * deltaTime;
		newPosition.x = std::clamp(newPosition.x, HalfSize().x, settings->screenWidth - HalfSize().x);
		sprite.setPosition(newPosition);
		direction = Direction::None;
	}

	void Platform::Reset()
	{
		direction = Direction::None;
		Settings* settings = Application::GetSettings();
		sprite.setPosition(settings->ScreenCenter().x, settings->screenHeight - HalfSize().y);
	}
}
