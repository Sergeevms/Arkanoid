#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace Arkanoid
{
	enum class Direction;

	class IPlatformObject : public virtual IGameObject
	{
	public:
		virtual void SetMovingDirection(const Direction direction) = 0;
	};

	class Platform : public IPlatformObject, public GameObject
	{
	public:
		Platform();
		virtual void SetMovingDirection(const Direction direction) override;
		virtual void Update(const float deltaTime) override;
		virtual void Reset() override;
		virtual bool CheckCollision(Collidable* object) override;
		virtual bool GetCollision(Collidable* object) const override;
		static bool CheckCollisionWithCircle(const sf::FloatRect platformRect, const sf::Vector2f circlePosition, const float circleRadius);
	protected:
		void OnHit() override {};
	private:
		Direction direction;
	};
}

