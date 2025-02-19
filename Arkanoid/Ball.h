#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "IObserver.h"

namespace Arkanoid
{
	class Ball : public GameObject, public IObservable
	{
	public:
		Ball();
		void Update(const float deltaTime) override;
		virtual bool GetCollision(Collidable* object) const override;
		void InvertX();
		void InvertY();
		virtual void Reset() override;
		void ChangeAngle(float angle);
		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
	protected:
		virtual void OnHit() override;
	private:
		//Direction vector
		sf::Vector2f direction;
		float previosAngle;
	};

	class BallSave : public GameObjectSave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const override;
		virtual void LoadFromFile(std::ifstream& istream) override;
	private:
		friend class Ball;
		sf::Vector2f direction;
		float previosAngle;
	};
}

