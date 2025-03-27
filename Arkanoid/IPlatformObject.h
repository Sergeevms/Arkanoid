#pragma once
#include "IGameObject.h"
namespace Arkanoid
{
	enum class Direction;

	class IPlatformObject : public virtual IGameObject
	{
	public:
		virtual void SetMovingDirection(const Direction direction) = 0;
	};
}

