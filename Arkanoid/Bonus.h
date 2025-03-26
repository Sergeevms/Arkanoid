#pragma once
#include <unordered_map>
#include <memory.h>
#include "GameObject.h"
#include "IDelayedAction.h"
#include "IObserver.h"

namespace Arkanoid
{
	enum class BonusType
	{
		platformSize,
		ballSpeed,
		oneHitBlock
	};
	
	class BonusFactory;

	class BonusSave : public GameObjectSave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const override;
		virtual void LoadFromFile(std::ifstream& istream) override;
	protected:
		float delayDuration = 0.f;
		float currentTime = 0.f;
		friend class Bonus;
		friend class IncreasePlatformBonus;
	};

	class IBonusObject :
		public virtual IGameObject
	{
	public:
		virtual void Apply(std::shared_ptr<IGameObject>& object) = 0;
		virtual BonusType GetType() = 0;
		virtual bool IsActivated() = 0;
		virtual bool IsToBeDestroyed() = 0;
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) = 0;
		virtual void OnEnding(std::shared_ptr<IGameObject>& object) const = 0;
	};

	class Bonus :
		public virtual IBonusObject, public GameObject, public IDelayedAction
	{
	public:
		Bonus(const sf::Vector2f position = {0.f, 0.f});
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void Update(const float deltaTime) override;
		bool GetCollision(Collidable* object) const override;
		virtual bool IsActivated() override;
		virtual bool IsToBeDestroyed() override;
		virtual void Apply(std::shared_ptr<IGameObject>& object) override;
		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) = 0;
		virtual void OnEnding(std::shared_ptr<IGameObject>& object) const;
		virtual void UpdateAction(float deltaTime) override {};
		virtual void FinalAction() override;
		virtual void OnHit() override;
		sf::Texture iconTexture;
		sf::Sprite iconSprite;
		std::vector<std::weak_ptr<IGameObject>> createdDecorators;
	};
	
	std::shared_ptr<Bonus> CreateBonusByType(const std::unordered_map<BonusType, std::unique_ptr<BonusFactory>>& bonusFactories, BonusType bonusType);

	class IncreasePlatformBonus : public Bonus
	{
	public:
		IncreasePlatformBonus(const sf::Vector2f position);
		virtual BonusType GetType() override { return BonusType::platformSize; };
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) override;
	};

	class IncreaseBallSpeedBonus : public Bonus
	{
	public:
		IncreaseBallSpeedBonus(const sf::Vector2f position);
		virtual BonusType GetType() override { return BonusType::ballSpeed; };
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) override;
	};

	class OneHitBlockBonus : public Bonus
	{
	public:
		OneHitBlockBonus(const sf::Vector2f position, std::weak_ptr<IObserver> observer);
		virtual BonusType GetType() override { return BonusType::oneHitBlock; };
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) override;
		std::weak_ptr<IObserver> observerToAdd;
	};
}

