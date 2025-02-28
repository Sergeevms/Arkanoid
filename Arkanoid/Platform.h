#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace Arkanoid
{
	enum class Direction;

	class Platform : public GameObject
	{
	public:
		Platform();
		void SetMovingDirection(const Direction direction);
		void Update(const float deltaTime) override;
		virtual void Reset() override;
		virtual bool CheckCollision(Collidable* object);
		virtual bool GetCollision(Collidable* object) const override;
		static bool CheckCollisionWithCircle(const sf::FloatRect platformRect, const sf::Vector2f circlePosition, const float circleRadius);
		void MultiplyWidth(float factor);
	protected:
		void OnHit() override {};
	private:
		Direction direction;
	};
}

