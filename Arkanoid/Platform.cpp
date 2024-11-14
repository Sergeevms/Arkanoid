#include "Platform.h"
#include "Utility.h"
#include "Application.h"
#include "Ball.h"

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
				ball->ChangeAngle(90.f - Application::GetSettings()->anglePlatformReboundChange * ballPositionOnPlatform);
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
			const Settings* settings = Application::GetSettings();
			const auto ballPosition = ball->GetPosition();
			const auto platformRect = GetRect();
			auto sqr = [](float x)
				{
					return x * x;
				};

			if (ballPosition.x < platformRect.left)
			{
				return sqr(ballPosition.x - platformRect.left) + sqr(ballPosition.y - platformRect.top) < sqr(settings->ballDiameter / 2.f);
			}

			if (ballPosition.x > platformRect.left + platformRect.width)
			{
				return sqr(ballPosition.x - platformRect.left - platformRect.width) + sqr(ballPosition.y - platformRect.top) < sqr(settings->ballDiameter / 2.f);
			}

			return std::fabs(ballPosition.y - platformRect.top) <= settings->ballDiameter / 2.f;
		}
	}
}
