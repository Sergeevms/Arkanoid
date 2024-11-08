#include "Ball.h"
#include "Platform.h"
#include "Utility.h"
#include "Application.h"
#include "PlayingState.h"

namespace Arkanoid
{
	Ball::Ball() : GameObject("ball.png")
	{
		Settings* settings = Application::GetSettings();
		SetScaleBySize(sprite, { settings->ballDiameter, settings->ballDiameter });
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		Reset();
	}

	void Ball::Update(const float deltaTime)
	{
		Settings* settings = Application::GetSettings();
		sf::Vector2f newPosition = sprite.getPosition() + direction * settings->ballSpeed * deltaTime;
		sprite.setPosition(newPosition);
		if (newPosition.x <= HalfSize().x || newPosition.x >= settings->screenWidth - HalfSize().x)
		{
			direction.x *= -1.f;
		}
		if (newPosition.y <= HalfSize().y)
		{
			direction.y *= -1.f;
		}
	}

	bool Ball::CheckCollision(const GameObject * object)
	{
		sf::FloatRect objectRect = object->GetRect();
		sf::Vector2f position = sprite.getPosition();

		bool collided = GetRect().intersects(objectRect);

		if (collided)
		{
			if (position.y < objectRect.top || position.y > objectRect.top + objectRect.height)
			{
				direction.y *= -1.f;
			}
			if (position.x < objectRect.left || position.x > objectRect.left + objectRect.width)
			{
				direction.x *= -1.f;
			}
		}

		return collided;
	}

	void Ball::Reset()
	{
		Settings* settings = Application::GetSettings();
		sprite.setPosition(settings->ScreenCenter());
		const float angleDegree = 45.f + rand() % 90;
		direction = DirectionVecFromDegree(angleDegree);
	}
}