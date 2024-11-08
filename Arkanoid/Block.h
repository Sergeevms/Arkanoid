#pragma once
#include "GameObject.h"

namespace Arkanoid
{
	class Block : public GameObject
	{
	public:
		Block(const sf::Vector2f& position);
		virtual void Update(const float deltaTime) {};
	};
}

