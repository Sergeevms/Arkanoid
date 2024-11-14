#pragma once
#include "GameObject.h"
#include "Collidable.h"

namespace Arkanoid
{
	class Block : public GameObject, public Collidable
	{
	public:
		Block(const sf::Vector2f& position, const sf::Color color = sf::Color::Red);
		virtual void Update(const float deltaTime) override {} ;
		virtual bool GetCollision(Collidable* object) const override;
		virtual bool IsBroken();
	protected:
		void OnHit() override;
	private:
		int HitCount = 1;
	};
}

