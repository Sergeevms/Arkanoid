#pragma once
#include<SFML/Graphics.hpp>
#include<unordered_map>

namespace Arkanoid
{
	enum class BonusType;
	class Bonus;
	class IObserver;

	class BonusFactory
	{
	public:
		virtual std::shared_ptr<Bonus> CreateBonus(const sf::Vector2f position = { 0.f, 0.f }) = 0;
		static std::unique_ptr<BonusFactory> CreateFactory(const BonusType);
		static std::shared_ptr<Bonus> TryToCreateRandomBonus(const std::unordered_map<BonusType, std::unique_ptr<BonusFactory>>& avaliableBonusFactories, const sf::Vector2f position);
	};

	class PlatformSizeBonusFactory : public BonusFactory
	{
	public:
		virtual std::shared_ptr<Bonus> CreateBonus(const sf::Vector2f position = {0.f, 0.f}) override;
	};

	class BallSpeedBonusFactory : public BonusFactory
	{
	public:
		virtual std::shared_ptr<Bonus> CreateBonus(const sf::Vector2f position = { 0.f, 0.f }) override;
	};

	class OneHitBlockBonusFactory : public BonusFactory
	{
	public:
		virtual std::shared_ptr<Bonus> CreateBonus(const sf::Vector2f position = { 0.f, 0.f }) override;
		virtual void SetObserver(std::weak_ptr<IObserver> observer);
	protected:
		std::weak_ptr<IObserver> observer;
	};
}

