#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "IPlatformObject.h"

namespace Arkanoid
{
	class Platform : public GameObject, public IPlatformObject
	{
	public:
		Platform();
		virtual void SetMovingDirection(const Direction direction) override;
		virtual void Update(const float deltaTime) override;
		virtual void Reset() override;
		virtual bool CheckCollision(Collidable* object) override;
		virtual bool GetCollision(Collidable* object) const override;
	protected:
		void OnHit() override {};
	private:
		Direction direction;
	};
}

