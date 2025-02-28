#include "BonusFactory.h"
#include "GameWorld.h"
#include "Randomizer.h"
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
}