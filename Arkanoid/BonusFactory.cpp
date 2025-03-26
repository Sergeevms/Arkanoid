#include "BonusFactory.h"
#include "GameWorld.h"
#include "Randomizer.h"
#include "IObserver.h"
#include "Bonus.h"

namespace Arkanoid
{
	std::unique_ptr<BonusFactory> BonusFactory::CreateFactory(const BonusType bonusType)
	{
		switch (bonusType)
		{
		case BonusType::platformSize:
			return std::make_unique<PlatformSizeBonusFactory>();
			break;
		case BonusType::ballSpeed:
			return std::make_unique<BallSpeedBonusFactory>();
			break;
		case BonusType::oneHitBlock:
			return std::make_unique<OneHitBlockBonusFactory>();
			break;
		default:
			return nullptr;
			break;
		}
	}

	std::shared_ptr<Bonus> BonusFactory::TryToCreateRandomBonus(const std::unordered_map<BonusType, std::unique_ptr<BonusFactory>>& avaliableBonusFactories, const sf::Vector2f position)
	{
		auto roll = random(0, 99);
		if (roll < GameWorld::GetWorld()->bonusAppearanceChance)
		{
			auto& avaliableBonuses = GameWorld::GetWorld()->avaliableBonuses;
			BonusType type = avaliableBonuses[rand() % avaliableBonuses.size()];
			if (avaliableBonusFactories.at(type))
			{
				return avaliableBonusFactories.at(type)->CreateBonus(position);
			}
		}
		return nullptr;
	}

	std::shared_ptr<Bonus> PlatformSizeBonusFactory::CreateBonus(const sf::Vector2f position)
	{
		return std::make_shared<IncreasePlatformBonus>(position);
	}

	std::shared_ptr<Bonus> BallSpeedBonusFactory::CreateBonus(const sf::Vector2f position)
	{
		return std::make_shared<IncreaseBallSpeedBonus>(position);
	}

	std::shared_ptr<Bonus> OneHitBlockBonusFactory::CreateBonus(const sf::Vector2f position)
	{
		return std::make_shared<OneHitBlockBonus>(position, observer);
	}

	void OneHitBlockBonusFactory::SetObserver(std::weak_ptr<IObserver> observer)
	{
		OneHitBlockBonusFactory::observer = observer;
	}
}