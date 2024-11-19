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
		GameWorld* world = GameWorld::GetWorld();
		SetScaleBySize(sprite, { world->ballDiameter, world->ballDiameter });
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		Reset();
	}

	void Ball::Update(const float deltaTime)
	{
		GameWorld* world = GameWorld::GetWorld();
		sf::Vector2f newPosition = sprite.getPosition() + direction * world->ballSpeed * deltaTime;
		sprite.setPosition(newPosition);
		if (newPosition.x < HalfSize().x || newPosition.x > world->screenWidth - HalfSize().x)
		{			
			bool movingDown = direction.y > 0.f;
			ChangeAngle(180.f - previosAngle);
			if (movingDown)
			{
				InvertY();
			}
		}
		if (newPosition.y < HalfSize().y || newPosition.y > world->screenHeight - HalfSize().y)
		{
			InvertY();
		}
	}	

	void Ball::OnHit()
	{
		Application::GetInstance().GetGame()->PlaySoundOnBallHit();
		float angleChange = GameWorld::GetWorld()->angleRandomChange;
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
		GameWorld* world = GameWorld::GetWorld();
		sprite.setPosition(world->ScreenCenter());
		previosAngle = 90.f;
		direction = DirectionVecFromDegree(previosAngle);
	}
}