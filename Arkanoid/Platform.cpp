#include "Platform.h"
#include "Utility.h"
#include "Application.h"
#include "Ball.h"
#include "Bonus.h"

namespace Arkanoid
{
	Platform::Platform() : GameObject("platform.png")
	{
		GameWorld* world = GameWorld::GetWorld();
		SetScaleBySize(sprite, world->platformSize);
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
		GameWorld* world = GameWorld::GetWorld();
		sf::Vector2f newPosition = sprite.getPosition() + directionVector * world->platformSpeed * deltaTime;
		newPosition.x = std::clamp(newPosition.x, HalfSize().x, world->screenWidth - HalfSize().x);
		sprite.setPosition(newPosition);
		direction = Direction::None;
	}

	void Platform::Reset()
	{
		direction = Direction::None;
		GameWorld* world = GameWorld::GetWorld();
		sprite.setPosition(world->ScreenCenter().x, world->screenHeight - HalfSize().y);
	}

	bool Platform::CheckCollision(Collidable* object)
	{		
		if (auto ball = dynamic_cast<IBallObject*>(object))
		{
			if (GetCollision(ball))
			{
				Application::GetInstance().GetGame()->PlaySoundOnBallHit();
				auto platformRect = GetRect();
				auto ballPositionOnPlatform = (ball->GetPosition().x - (platformRect.left + platformRect.width / 2.f)) / (platformRect.width / 2.f);
				ball->ChangeAngle(90.f - GameWorld::GetWorld()->anglePlatformReboundChange * ballPositionOnPlatform);
				return true;
			}
			return false;
		}
		else if (auto bonus = dynamic_cast<Bonus*>(object))
		{
			if (GetCollision(bonus))
			{
				bonus->CheckCollision(this);
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	bool Platform::GetCollision(Collidable* object) const
	{
		if (auto ball = dynamic_cast<IBallObject*>(object))
		{
			
			return CheckCollisionWithCircle(GetRect(), ball->GetPosition(), GameWorld::GetWorld()->ballDiameter / 2.f); 
		}
		else if (auto bonus = dynamic_cast<Bonus*>(object))
		{
			return (!bonus->IsActivated()) && CheckCollisionWithCircle(GetRect(), bonus->GetPosition(), GameWorld::GetWorld()->bonusSize / 2.f);
		}
		else
		{
			return false;
		}
	}

	bool Platform::CheckCollisionWithCircle(const sf::FloatRect platformRect, const sf::Vector2f circlePosition, const float circleRadius)
	{
		auto sqr = [](float x)
			{
				return x * x;
			};

		if (circlePosition.x < platformRect.left)
		{
			return sqr(circlePosition.x - platformRect.left) + sqr(circlePosition.y - platformRect.top) < sqr(circleRadius);
		}

		if (circlePosition.x > platformRect.left + platformRect.width)
		{
			return sqr(circlePosition.x - platformRect.left - platformRect.width) + sqr(circlePosition.y - platformRect.top) < sqr(circleRadius);
		}

		return std::fabs(circlePosition.y - platformRect.top) <= circleRadius;
	}
}
