#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace Arkanoid
{
	class Platform;

	class Ball : public GameObject
	{
	public:
		Ball();
		void Update(const float deltaTime) override;
		void CheckPlatformCollision(const Platform* platform);
		void Reset();
	private:
		sf::Vector2f direction;
	};
}

