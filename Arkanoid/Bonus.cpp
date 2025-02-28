#include "Bonus.h"
#include "Utility.h"
#include "GameWorld.h"
#include "Platform.h"
#include "BonusFactory.h"

namespace Arkanoid
{
	std::shared_ptr<BonusSave> CreateEmptySaveByBonusType(BonusType type)
	{
		switch (type)
		{
		default:
			return std::make_shared<BonusSave>();
			break;
		}
	}

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
		}
	}

	void Bonus::Update(const float deltaTime)
	{
		if (delayDuration <= 0.f)
		{
			auto world = GameWorld::GetWorld();
			sf::Vector2f newPosition = sprite.getPosition();
			newPosition.y += deltaTime * world->bonusSpeed;
			if (newPosition.y > world->screenHeight - world->platformSize.y)
			{
				Emit();
			}
			sprite.setPosition(newPosition);
		}
		else
		{
			UpdateTimer(deltaTime);
		}
	}

	bool Bonus::GetCollision(Collidable* object) const
	{
		auto gameObject = dynamic_cast<GameObject*>(object);
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

	void Bonus::Apply(std::weak_ptr<GameObject> object) const
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

	void Bonus::FinalAction()
	{
		Emit();
	}

	IncreasePlatformBonus::IncreasePlatformBonus(const sf::Vector2f position) : Bonus(position)
	{
		auto world = GameWorld::GetWorld();
		sprite.setColor(world->bonusColors[BonusType::platformSize]);
		LoadTexture("platform.png", miniPlatformTexture);
		miniPlatformSprite.setTexture(miniPlatformTexture);
		SetScaleBySize(miniPlatformSprite, { world->bonusSize - 2.f, 4.f });
		SetOriginByRelative(miniPlatformSprite, relativePositions.at(RelativePosition::Center));
	}

	void IncreasePlatformBonus::Draw(sf::RenderWindow& window) const
	{
		if (delayDuration <= 0.f)
		{
			Bonus::Draw(window);
			window.draw(miniPlatformSprite);
		}
	}

	void IncreasePlatformBonus::Update(const float deltaTime)
	{
		Bonus::Update(deltaTime);
		miniPlatformSprite.setPosition(sprite.getPosition());
	}

	void IncreasePlatformBonus::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto bonusSave = std::dynamic_pointer_cast<BonusSave> (save))
		{
			GameObject::LoadState(bonusSave);
			currentTime = bonusSave->currentTime;
			delayDuration = bonusSave->delayDuration;
		}
	}

	void IncreasePlatformBonus::OnActivation(std::weak_ptr<GameObject> object) const
	{
		if (auto platform = std::dynamic_pointer_cast<Platform> (object.lock()); platform)
		{
			platform->MultiplyWidth(GameWorld::GetWorld()->platformBonusFactor);
		}
	}

	void IncreasePlatformBonus::OnEnding(std::weak_ptr<GameObject> object) const
	{
		if (auto platform = std::dynamic_pointer_cast<Platform> (object.lock()); platform)
		{
			platform->MultiplyWidth(1.f / GameWorld::GetWorld()->platformBonusFactor);
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
}
