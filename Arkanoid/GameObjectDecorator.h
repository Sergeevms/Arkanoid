#pragma once
#pragma 
#include "IGameObject.h"

namespace Arkanoid
{
	class GameObjectDecorator : public virtual IGameObject, public std::enable_shared_from_this<GameObjectDecorator>
	{
	public:
		GameObjectDecorator(std::shared_ptr<IGameObject> object) : wrappedObject(object) {};
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

	class SpriteChangedDecorator : public virtual GameObjectDecorator
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
}