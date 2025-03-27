#include "Ball.h"
#include "Utility.h"
#include "Application.h"
#include "Randomizer.h"

namespace Arkanoid
{
	Ball::Ball() : GameObject("ball.png")
	{
		GameWorld* world = GameWorld::GetWorld();
		SetScaleBySize(sprite, { world->ballDiameter, world->ballDiameter });
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		Reset();
	}

	void Ball::Update(const float deltaTime)
	{
		GameWorld* world = GameWorld::GetWorld();
		sf::Vector2f newPosition = sprite.getPosition() + direction * world->ballSpeed * deltaTime;
		if (newPosition.x < HalfSize().x || newPosition.x > world->screenWidth - HalfSize().x)
		{
			//Correcting position to prevent ball from "sticking" to the wall
			newPosition.x = std::clamp(newPosition.x, HalfSize().x, world->screenWidth - HalfSize().x);
			bool movingDown = direction.y > 0.f;
			ChangeAngle(180.f - previosAngle);
			if (movingDown)
			{
				InvertY();
			}
		}
		if (newPosition.y < HalfSize().y)
		{
			InvertY();
		}
		sprite.setPosition(newPosition);
		Emit();
	}	

	void Ball::OnHit()
	{
		Application::GetInstance().GetGame()->PlaySoundOnBallHit();
		float angleChange = GameWorld::GetWorld()->angleRandomChange;
		previosAngle += random<float>(-1 * angleChange, angleChange);
		ChangeAngle(previosAngle);
	}

	void Ball::ChangeAngle(float angle)
	{
		previosAngle = angle;
		const auto pi = std::acos(-1.f);
		direction.x = (angle / abs(angle)) * std::cos(pi / 180.f * angle);
		direction.y = -1 * abs(std::sin(pi / 180.f * angle));
	}

	std::shared_ptr<ISave> Ball::SaveState() const
	{		
		std::shared_ptr<BallSave> ballSave = std::make_shared<BallSave>();
		SaveState(ballSave);
		return ballSave;
	}

	void Ball::SaveState(std::shared_ptr<ISave> save) const
	{
		GameObject::SaveState(save);
		if (auto ballSave = std::dynamic_pointer_cast<BallSave>(save))
		{
			ballSave->direction = direction;
			ballSave->previosAngle = previosAngle;
		}
	}

	void Ball::LoadState(const std::shared_ptr<ISave> save)
	{
		GameObject::LoadState(save);
		if (auto ballSave = std::dynamic_pointer_cast<BallSave>(save))
		{
			direction = ballSave->direction;
			previosAngle = ballSave->previosAngle;
		}
	}

	void Ball::InvertX()
	{
		direction.x *= -1.f;
	}

	void Ball::InvertY()
	{
		direction.y *= -1.f;
	}

	bool Ball::GetCollision(Collidable *object) const
	{
		auto gameObject = dynamic_cast<IGameObject*>(object);
		assert(gameObject);
		return GetRect().intersects(gameObject->GetRect());
	}

	void Ball::Reset()
	{
		GameWorld* world = GameWorld::GetWorld();
		sprite.setPosition(world->ScreenCenter());
		previosAngle = 90.f;
		direction = DirectionVecFromDegree(previosAngle);
	}

	void BallSave::SaveToFile(std::ofstream& ostream) const
	{
		GameObjectSave::SaveToFile(ostream);
		ostream << direction.x << " " << direction.y << " " << previosAngle << " ";
	}

	void BallSave::LoadFromFile(std::ifstream& istream)
	{
		GameObjectSave::LoadFromFile(istream);
		istream >> direction.x >> direction.y >> previosAngle;
	}
}