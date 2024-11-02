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
		void Reset();
	private:
		Direction direction;
	};
}

