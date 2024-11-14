#include "GameObject.h"
#include "Utility.h"

namespace Arkanoid
{
	GameObject::GameObject(const std::string textureName)
	{
		LoadTexture(textureName, texture);
		sprite.setTexture(texture);
	}

	void GameObject::Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	sf::FloatRect GameObject::GetRect() const
	{
		return sprite.getGlobalBounds();
	}

	sf::Vector2f GameObject::GetPosition() const
	{
		return sprite.getPosition();
	}

	sf::Vector2f GameObject::HalfSize() const
	{
		sf::Vector2f size{ sprite.getGlobalBounds().width, sprite.getGlobalBounds().height };
		return size / 2.f;
	}

}