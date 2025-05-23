#pragma once
#pragma warning( disable : 4250 )
#include <memory>
#include "SFML/Graphics.hpp"
#include "IGameObject.h"
#include "ISaveable.h"

namespace Arkanoid
{	
	class GameObject : public virtual IGameObject, public virtual ISaveable//, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(const std::string textureName);
		virtual ~GameObject() = default;
		virtual void Draw(sf::RenderWindow& window) const override;
		virtual void Update(const float deltaTime) { };
		virtual sf::FloatRect GetRect() const override;
		virtual sf::Vector2f GetPosition() const override;
		virtual void SetPosition(const sf::Vector2f newPosition) override;
		virtual sf::Vector2f HalfSize() const override;
		virtual void Reset() { };
		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
		virtual sf::Color GetColor() override;
	protected:
		sf::Texture texture;
		sf::Sprite sprite;
		friend class GameObjectSave;
	};
		
	class GameObjectSave : public ISave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const override;
		virtual void LoadFromFile(std::ifstream& istream) override;
	protected:
		friend class GameObject;
		sf::Vector2f position;
		sf::Vector2f size;
	};
}

