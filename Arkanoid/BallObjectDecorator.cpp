#include "BallObjectDecorator.h"
#include "GameWorld.h"

namespace Arkanoid
{
	BallObjectDecorator::BallObjectDecorator(std::shared_ptr<IBallObject> object) : GameObjectDecorator(object)
	{
	}

	BallSpeedDecorator::BallSpeedDecorator(std::shared_ptr<IBallObject> object, float updateTimeCoefficient)
		: GameObjectDecorator(object), BallObjectDecorator(object), SpriteChangedDecorator(object, "Ball.png"), coefficient(updateTimeCoefficient)
	{
	}

	void BallSpeedDecorator::Update(const float deltaTime)
	{
		SpriteChangedDecorator::Update(deltaTime * coefficient);
		sprite.setColor(GameWorld::GetWorld()->acceleratedBallColor);
	}
}