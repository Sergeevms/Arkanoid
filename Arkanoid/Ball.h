#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "IObserver.h"

namespace Arkanoid
{
	class IBallObject : public virtual IGameObject
	{
	public:
		virtual void InvertX() = 0;
		virtual void InvertY() = 0;
		virtual void ChangeAngle(float angle) = 0;
	};

	class Ball : public GameObject, public IBallObject
	{
	public:
		Ball();
		void Update(const float deltaTime) override;
		virtual bool GetCollision(Collidable* object) const override;
		virtual void InvertX() override;
		virtual void InvertY() override;
		virtual void Reset() override;
		virtual void ChangeAngle(float angle) override;
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

