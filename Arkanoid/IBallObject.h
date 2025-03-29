#pragma once
#include "IGameObject.h"
namespace Arkanoid
{
	class IBallObject : public virtual IGameObject
	{
	public:
		virtual void InvertX() = 0;
		virtual void InvertY() = 0;
		virtual void ChangeAngle(float angle) = 0;
	};
}