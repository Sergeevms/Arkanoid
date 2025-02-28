#pragma once
#include "GameObject.h"
#include "IDelayedAction.h"
#include "IObserver.h"
#include <unordered_map>

namespace Arkanoid
{
	enum class BonusType
	{
		platformSize
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

	std::shared_ptr<BonusSave> CreateEmptySaveByBonusType(BonusType);

	class Bonus :
		public GameObject, public IDelayedAction, public IObservable
	{
	public:
		Bonus(const sf::Vector2f position = {0.f, 0.f});
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void Update(const float deltaTime) override;
		bool GetCollision(Collidable* object) const override;
		virtual void OnHit() override;
		virtual bool IsActivated();
		virtual void Apply(std::weak_ptr <GameObject> object) const;
		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
		virtual BonusType GetType() = 0;
	protected:
		virtual void OnActivation(std::weak_ptr <GameObject> object) const = 0;
		virtual void OnEnding(std::weak_ptr <GameObject> object) const = 0;
		virtual void UpdateAction(float deltaTime) override {};
		virtual void FinalAction() override;
	};
	
	std::shared_ptr<Bonus> CreateBonusByType(const std::unordered_map<BonusType, std::unique_ptr<BonusFactory>>& bonusFactories, BonusType bonusType);

	class IncreasePlatformBonus : public Bonus
	{
	public:
		IncreasePlatformBonus(const sf::Vector2f position);
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void Update(const float deltaTime) override;
		virtual BonusType GetType() override{ return BonusType::platformSize; };
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
	protected:
		virtual void OnActivation(std::weak_ptr <GameObject> object) const override;
		virtual void OnEnding(std::weak_ptr <GameObject> object) const override;
	private:
		sf::Texture miniPlatformTexture;
		sf::Sprite miniPlatformSprite;
	};
}

