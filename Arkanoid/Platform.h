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
	protected:
		void OnHit() override {};
	private:
		Direction direction;
	};
}

