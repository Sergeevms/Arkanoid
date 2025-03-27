#include "Bonus.h"
#include "Utility.h"
#include "GameWorld.h"
#include "BonusFactory.h"
#include "BlockObjectDecorator.h"
#include "BallObjectDecorator.h"
#include "PlatformObjectDecorator.h"

namespace Arkanoid
{

	Bonus::Bonus(const sf::Vector2f position) : GameObject("ball.png")
	{
		SetScaleBySize(sprite, { GameWorld::GetWorld()->bonusSize,GameWorld::GetWorld()->bonusSize });
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sprite.setPosition(position);
	}

	std::shared_ptr<Bonus> CreateBonusByType(const std::unordered_map<BonusType, std::unique_ptr<BonusFactory>>& bonusFactories, BonusType bonusType)
	{
		if (bonusFactories.contains(bonusType))
		{
			return bonusFactories.at(bonusType)->CreateBonus();
		}
		else
		{
			return nullptr;
		}
	}

	void Bonus::Draw(sf::RenderWindow& window) const
	{
		if (delayDuration <= 0.f)
		{
			window.draw(sprite);
			window.draw(iconSprite);
		}
	}

	void Bonus::Update(const float deltaTime)
	{
		if (delayDuration <= 0.f)
		{
			auto world = GameWorld::GetWorld();
			sf::Vector2f newPosition = sprite.getPosition();
			newPosition.y += deltaTime * world->bonusSpeed;
			sprite.setPosition(newPosition);
			iconSprite.setPosition(newPosition);
			Emit();
		}
		else
		{
			UpdateTimer(deltaTime);
		}
	}

	bool Bonus::GetCollision(Collidable* object) const
	{
		auto gameObject = dynamic_cast<IGameObject*>(object);
		assert(gameObject);
		return GetRect().intersects(gameObject->GetRect());
	}

	void Bonus::OnHit()
	{
		StartTimer(GameWorld::GetWorld()->bonusDuration);
		Emit();
	}

	bool Bonus::IsActivated()
	{
		return delayDuration > 0.f;
	}

	bool Bonus::IsToBeDestroyed()
	{
		return ((GetPosition().y > GameWorld::GetWorld()->screenHeight - GameWorld::GetWorld()->platformSize.y / 2.f) ||
			(delayDuration > 0.f && currentTime <= 0.f));
	}

	void Bonus::Apply(std::shared_ptr<IGameObject>& object)
	{
		if (currentTime > 0.f)
		{
			OnActivation(object);
		}
		else
		{
			OnEnding(object);
		}
	}

	std::shared_ptr<ISave> Bonus::SaveState() const
	{
		auto bonusSave = std::make_shared<BonusSave>();
		SaveState(bonusSave);
		return bonusSave;
	}

	void Bonus::SaveState(std::shared_ptr<ISave> save) const
	{
		if (auto bonusSave = std::dynamic_pointer_cast<BonusSave>(save))
		{
			GameObject::SaveState(bonusSave);
			bonusSave->currentTime = currentTime;
			bonusSave->delayDuration = delayDuration;
		}
	}

	void Bonus::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto bonusSave = std::dynamic_pointer_cast<BonusSave> (save))
		{
			GameObject::LoadState(bonusSave);
			currentTime = bonusSave->currentTime;
			delayDuration = bonusSave->delayDuration;
			if (currentTime > 0.f)
			{
				Emit();
			}
		}
	}

	void Bonus::OnEnding(std::shared_ptr<IGameObject>& object) const
	{
		if (auto decorator = std::dynamic_pointer_cast<GameObjectDecorator>(object))
		{
			for (bool decoratorRevomed = false; auto& decoratorToRemove : createdDecorators)
			{
				if (!decoratorToRemove.expired())
				{
					object = decorator->RemoveDecorator(decoratorToRemove, decoratorRevomed);
					if (decoratorRevomed)
					{
						break;
					}
				}
			}
		}		
	}

	void Bonus::FinalAction()
	{
		Emit();
	}

	IncreasePlatformBonus::IncreasePlatformBonus(const sf::Vector2f position) : Bonus(position)
	{
		auto world = GameWorld::GetWorld();
		sprite.setColor(world->bonusColors[BonusType::platformSize]);
		LoadTexture("platform.png", iconTexture);
		iconSprite.setTexture(iconTexture);
		SetScaleBySize(iconSprite, { world->bonusSize - 2.f, 4.f });
		SetOriginByRelative(iconSprite, relativePositions.at(RelativePosition::Center));
	}

	void IncreasePlatformBonus::OnActivation(std::shared_ptr<IGameObject>& object)
	{
		if (auto platform = std::dynamic_pointer_cast<IPlatformObject> (object); platform)
		{
			std::shared_ptr<IGameObject> widePlatform = std::make_shared<PlatformSizeDecorator>(platform, sf::Vector2f(GameWorld::GetWorld()->platformBonusFactor, 1.f));
			createdDecorators.push_back(widePlatform);
			object = widePlatform;
		}
	}

	void BonusSave::SaveToFile(std::ofstream& ostream) const
	{
		GameObjectSave::SaveToFile(ostream);
		ostream << currentTime << " " << delayDuration << " " ;
	}

	void BonusSave::LoadFromFile(std::ifstream& istream)
	{
		GameObjectSave::LoadFromFile(istream);
		istream >> currentTime >> delayDuration;
	}

	IncreaseBallSpeedBonus::IncreaseBallSpeedBonus(const sf::Vector2f position) : Bonus(position)
	{
		auto world = GameWorld::GetWorld();
		sprite.setColor(world->bonusColors[BonusType::ballSpeed]);
		LoadTexture("ball.png", iconTexture);
		iconSprite.setTexture(iconTexture);
		SetScaleBySize(iconSprite, { world->bonusSize * 0.5f, world->bonusSize * 0.5f });
		SetOriginByRelative(iconSprite, relativePositions.at(RelativePosition::Center));
	}

	void IncreaseBallSpeedBonus::OnActivation(std::shared_ptr<IGameObject>& object)
	{
		if (auto ball = std::dynamic_pointer_cast<IBallObject> (object); ball)
		{
			std::shared_ptr<IGameObject> fastBall = std::make_shared<BallSpeedDecorator>(ball, GameWorld::GetWorld()->ballSpeedBonusFactor);
			createdDecorators.push_back(fastBall);
			object = fastBall;
		}
	}

	OneHitBlockBonus::OneHitBlockBonus(const sf::Vector2f position, std::weak_ptr<IObserver> observer) : Bonus(position), observerToAdd(observer)
	{
		auto world = GameWorld::GetWorld();
		sprite.setColor(world->bonusColors[BonusType::oneHitBlock]);
		LoadTexture("platform.png", iconTexture);
		iconSprite.setTexture(iconTexture);
		iconSprite.setColor(world->oneHitBlockColor);
		SetScaleBySize(iconSprite, { world->bonusSize / 2.f, world->bonusSize / 4.f });
		SetOriginByRelative(iconSprite, relativePositions.at(RelativePosition::Center));
	}

	void OneHitBlockBonus::OnActivation(std::shared_ptr<IGameObject>& object)
	{
		if (auto block = std::dynamic_pointer_cast<IBlockObject> (object); block)
		{
			std::shared_ptr<IGameObject> oneHitBlock = std::make_shared<OneHitBlockDecorator>(block);
			createdDecorators.push_back(oneHitBlock);
			object = oneHitBlock;
			oneHitBlock->AddObserver(observerToAdd);
		}
	}
}
