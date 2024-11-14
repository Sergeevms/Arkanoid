#include "Ball.h"
#include "Platform.h"
#include "Utility.h"
#include "Application.h"
#include "PlayingState.h"
#include "Randomizer.h"

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
		if (newPosition.x < HalfSize().x || newPosition.x > settings->screenWidth - HalfSize().x)
		{
			InvertX();
		}
		if (newPosition.y < HalfSize().y || newPosition.y > settings->screenHeight - HalfSize().y)
		{
			InvertY();
		}
	}	

	void Ball::OnHit()
	{
		float angleChange = Application::GetSettings()->angleRandomChange;
		previosAngle += random<float>(-1 * angleChange, angleChange);
		ChangeAngle(previosAngle);
	}

	void Ball::ChangeAngle(float angle)
	{
		previosAngle = angle;
		const auto pi = std::acos(-1.f);
		direction.x = (angle / abs(angle)) * std::cos(pi / 180.f * angle);
		direction.y = -1 * abs(std::sin(pi / 180.f * angle));
	}

	void Ball::InvertX()
	{
		direction.x *= -1.f;
	}

	void Ball::InvertY()
	{
		direction.y *= -1.f;
	}

	bool Ball::GetCollision(Collidable *object) const
	{
		auto gameObject = dynamic_cast<GameObject*>(object);
		assert(gameObject);
		return GetRect().intersects(gameObject->GetRect());
	}

	void Ball::Reset()
	{
		Settings* settings = Application::GetSettings();
		sprite.setPosition(settings->ScreenCenter());
		previosAngle = 90.f;
		direction = DirectionVecFromDegree(previosAngle);
	}
}