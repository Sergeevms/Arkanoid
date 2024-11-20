#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Collidable.h"
#include "IObserver.h"

namespace Arkanoid
{
	class Ball : public GameObject, public Collidable, public IObservable
	{
	public:
		Ball();
		void Update(const float deltaTime) override;
		virtual bool GetCollision(Collidable* object) const override;
		void InvertX();
		void InvertY();
		virtual void Reset() override;
		void ChangeAngle(float angle);
	protected:
		virtual void OnHit() override;
	private:
		//Direction vector
		sf::Vector2f direction;
		float previosAngle;
	};
}

