#pragma once
#pragma warning( push )
#pragma warning( disable : 4250)

#include "GameObjectDecorator.h"
#include "IBallObject.h"

namespace Arkanoid
{
	class BallObjectDecorator : public virtual IBallObject, public virtual GameObjectDecorator
	{
	public:
		BallObjectDecorator(std::shared_ptr<IBallObject> object);
		virtual void InvertX() override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->InvertX(); };
		virtual void InvertY() override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->InvertY(); };
		virtual void ChangeAngle(float angle) override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->ChangeAngle(angle); };
	};

	class BallSpeedDecorator : public BallObjectDecorator, public SpriteChangedDecorator
	{
	public:
		BallSpeedDecorator(std::shared_ptr<IBallObject> object, float updateTimeCoefficient);
		virtual void Update(const float deltaTime) override;
	protected:
		float coefficient = 1.f;
	};
}

#pragma warning( pop )
