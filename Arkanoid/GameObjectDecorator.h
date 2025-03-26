#pragma once
#include "GameObject.h"
#include "Ball.h"
#include "Platform.h"
#include "Block.h"

namespace Arkanoid
{
	class BaseGameObjectDecorator : public virtual IGameObject, public std::enable_shared_from_this<BaseGameObjectDecorator>
	{
	public:
		BaseGameObjectDecorator(std::shared_ptr<IGameObject> object) : wrappedObject(object) {};
		virtual void Draw(sf::RenderWindow& window) const override { wrappedObject->Draw(window); };
		virtual void Update(const float deltaTime) override { wrappedObject->Update(deltaTime); };
		virtual sf::FloatRect GetRect() const override { return wrappedObject->GetRect(); };
		virtual sf::Vector2f GetPosition() const override { return wrappedObject->GetPosition(); };
		virtual void SetPosition(const sf::Vector2f newPosition) override { wrappedObject->SetPosition(newPosition); };
		virtual sf::Vector2f HalfSize() const override { return wrappedObject->HalfSize(); };
		virtual void Reset() override { wrappedObject->Reset(); };
		virtual sf::Color GetColor() override { return  wrappedObject->GetColor(); };
		std::shared_ptr<IGameObject> GetObjectPointer() { return wrappedObject; };
		virtual std::shared_ptr<IGameObject> RemoveDecorator(std::weak_ptr<IGameObject> object, bool & decoratorRemoved);
		virtual bool GetCollision(Collidable* object) const override { return wrappedObject->GetCollision(object); };
		virtual void OnHit() override { wrappedObject->OnHit(); };
		virtual void AddObserver(std::weak_ptr<IObserver> observer) override { wrappedObject->AddObserver(observer); };
		virtual std::shared_ptr<IObservable> GetObservablePtr() override { return wrappedObject->GetObservablePtr(); };
		virtual std::shared_ptr<ISave> SaveState() const override { return wrappedObject->SaveState(); };
		virtual void SaveState(std::shared_ptr<ISave> save) const override { wrappedObject->SaveState(save); };
		virtual void LoadState(const std::shared_ptr<ISave> save) override { wrappedObject->LoadState(save); };
	protected:
		std::shared_ptr<IGameObject> wrappedObject;
	};

	class SpriteChangedDecorator : public virtual BaseGameObjectDecorator
	{
	public:
		SpriteChangedDecorator(std::shared_ptr<IGameObject> object, const std::string textureName);
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void Update(const float deltaTime) override;
		virtual sf::FloatRect GetRect() const override;
		virtual sf::Vector2f HalfSize() const override;
	protected:
		sf::Texture texture;
		sf::Sprite sprite;
	};

	class BallDecorator : public virtual IBallObject, public virtual BaseGameObjectDecorator
	{
	public:
		BallDecorator(std::shared_ptr<IGameObject> object);
		virtual void InvertX() override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->InvertX(); };
		virtual void InvertY() override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->InvertY(); };
		virtual void ChangeAngle(float angle) override { std::dynamic_pointer_cast<IBallObject>(wrappedObject)->ChangeAngle(angle); };
	};

	class PlatformDecorator : public virtual IPlatformObject, public virtual BaseGameObjectDecorator
	{
	public:
		PlatformDecorator(std::shared_ptr <IGameObject> object);
		virtual void SetMovingDirection(const Direction direction) override { std::dynamic_pointer_cast<IPlatformObject>(wrappedObject)->SetMovingDirection(direction); };
	};

	class BlockDecorator : public virtual IBlockObject, public virtual BaseGameObjectDecorator
	{
	public:
		BlockDecorator(std::shared_ptr<IGameObject> object);
		virtual bool IsBroken() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBroken(); };
		virtual bool IsBreakable() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBreakable(); };
		virtual bool IsBallReboundable() const { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->IsBallReboundable(); };
		virtual int GetScore() const { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->GetScore(); };
		virtual BlockType GetBlockType() const override { return std::dynamic_pointer_cast<IBlockObject>(wrappedObject)->GetBlockType(); };
	};

	class PlatformSizeDecorator : public virtual SpriteChangedDecorator, public virtual PlatformDecorator
	{
	public:
		PlatformSizeDecorator(std::shared_ptr<IGameObject> object, const sf::Vector2f scale);
		virtual void Update(const float deltaTime) override;
		bool GetCollision(Collidable* object) const override;
	protected:
		sf::Vector2f scale;
	};

	class BallSpeedDecorator : public virtual BallDecorator, public virtual SpriteChangedDecorator
	{
	public:
		BallSpeedDecorator(std::shared_ptr<IGameObject> object, float updateTimeCoefficient);
		virtual void Update(const float deltaTime) override;
	protected:
		float coefficient = 1.f;
	};

	class OneHitBlockDecorator : public virtual BlockDecorator, public virtual SpriteChangedDecorator
	{
	public:
		OneHitBlockDecorator(std::shared_ptr<IGameObject> object);
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void OnHit() override;
		virtual bool IsBroken() const override;
		virtual bool IsBreakable() const override;
		virtual void Update(const float deltaTime) override;
		virtual void AddObserver(std::weak_ptr<IObserver> observer) override;
		virtual std::shared_ptr<IObservable> GetObservablePtr() override;
	protected:
		bool isBroken = false;
	};
}