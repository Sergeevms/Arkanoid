#include "Ball.h"
#include "Platform.h"
#include "Utility.h"
#include "Application.h"

namespace Arkanoid
{
	Ball::Ball()
	{
		LoadTexture("ball.png", texture);
		sprite.setTexture(texture);
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
		if (newPosition.y <= HalfSize().y || newPosition.y >= settings->screenHeight - HalfSize().y)
		{
			direction.y *= -1.f;
		}
	}

	void Ball::Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	void Ball::CheckPlatformCollision(const Platform* platform)
	{
		sf::FloatRect platformRect = platform->GetRect();
		sf::Vector2f position = sprite.getPosition();
		bool collided = false;
		auto sqr = [](float x) {return x * x; };

		if (position.x < platformRect.left)
		{
			collided = sqr(position.x - platformRect.left) + sqr(position.y - platformRect.top) < sqr(HalfSize().x);
		}
		else if (position.x > platformRect.left + platformRect.width)
		{
			collided = sqr(position.x - platformRect.left - platformRect.width) + sqr(position.y - platformRect.top) < sqr(HalfSize().x);
		}
		else
		{
			collided = std::fabs(position.y - platformRect.top) <= HalfSize().y;
		}

		//Second condition to prevent catching ball inside platform while there no game over condition
		if (collided && direction.y >= 0.f)
		{
			direction.y *= -1.f;
		}
	}

	void Ball::Reset()
	{
		Settings* settings = Application::GetSettings();
		sprite.setPosition(settings->ScreenCenter());
		const float angleDegree = 45.f + rand() % 90;
		direction = DirectionVecFromDegree(angleDegree);
	}

	sf::Vector2f Ball::HalfSize() const
	{
		sf::Vector2f size{ sprite.getLocalBounds().width, sprite.getLocalBounds().height };
		return size / 2.f;
	}
}