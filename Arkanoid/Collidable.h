#pragma once
#include <assert.h>

namespace Arkanoid
{
	class Collidable
	{
	public:
		virtual bool GetCollision(Collidable* object) const = 0;
		virtual bool CheckCollision(Collidable* object)
		{
			if (GetCollision(object))
			{
				OnHit();
				object->OnHit();
				return true;
			}
			return false;
		}
	protected:
		virtual void OnHit() = 0;
	};
}