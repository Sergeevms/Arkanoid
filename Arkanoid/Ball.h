#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Collidable.h"

namespace Arkanoid
{
	class Ball : public GameObject, public Collidable
	{
	public:
		Ball();
		void Update(const float deltaTime) override;
		virtual bool GetCollision(Collidable* object) const override;
		void InvertX();
		void InvertY();
		void Reset();
		void ChangeAngle(float angle);
	protected:
		virtual void OnHit() override;
	private:
		//Direction vector
		sf::Vector2f direction;
		float previosAngle;
	};
}

