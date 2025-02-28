#pragma once
#include "SFML/Graphics.hpp"
#include "Collidable.h"
#include "ISaveable.h"

namespace Arkanoid
{

	class GameObject : public Collidable, public ISaveable
	{
	public:
		GameObject(const std::string textureName);
		virtual ~GameObject() = default;
		virtual void Draw(sf::RenderWindow& window) const;
		virtual void Update(const float deltaTime) = 0;
		sf::FloatRect GetRect() const;
		sf::Vector2f GetPosition() const;
		sf::Vector2f HalfSize() const;
		virtual void Reset() { };
		virtual std::shared_ptr<ISave> SaveState() const override;
		virtual void SaveState(std::shared_ptr<ISave> save) const override;
		virtual void LoadState(const std::shared_ptr<ISave> save) override;
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

