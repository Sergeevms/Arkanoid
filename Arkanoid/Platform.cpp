#include "Platform.h"
#include "Utility.h"
#include "Application.h"
#include "Ball.h"

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
		auto ball = dynamic_cast<Ball*>(object);
		if (!ball)
		{
			return false;
		}
		else
		{
			if (GetCollision(ball))
			{
				auto platformRect = GetRect();
				auto ballPositionOnPlatform = (ball->GetPosition().x - (platformRect.left + platformRect.width / 2.f)) / (platformRect.width / 2.f);
				ball->ChangeAngle(90.f - GameWorld::GetWorld()->anglePlatformReboundChange * ballPositionOnPlatform);
				return true;
			}
			return false;
		}
	}

	bool Platform::GetCollision(Collidable* object) const
	{
		auto ball = dynamic_cast<Ball*>(object);

		if (!ball)
		{
			return false;
		}
		else
		{
			const GameWorld* world = GameWorld::GetWorld();
			const auto ballPosition = ball->GetPosition();
			const auto platformRect = GetRect();
			auto sqr = [](float x)
				{
					return x * x;
				};

			if (ballPosition.x < platformRect.left)
			{
				return sqr(ballPosition.x - platformRect.left) + sqr(ballPosition.y - platformRect.top) < sqr(world->ballDiameter / 2.f);
			}

			if (ballPosition.x > platformRect.left + platformRect.width)
			{
				return sqr(ballPosition.x - platformRect.left - platformRect.width) + sqr(ballPosition.y - platformRect.top) < sqr(world->ballDiameter / 2.f);
			}

			return std::fabs(ballPosition.y - platformRect.top) <= world->ballDiameter / 2.f;
		}
	}
}
