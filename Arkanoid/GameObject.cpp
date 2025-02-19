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

	std::shared_ptr<ISave> GameObject::SaveState() const
	{
		auto save = std::make_shared<GameObjectSave>();
		SaveState(save);
		return save;
	}

	void GameObject::SaveState(std::shared_ptr<ISave> save) const
	{
		if (auto objectSave = std::dynamic_pointer_cast<GameObjectSave> (save))
		{
			objectSave->position = GetPosition();
			objectSave->size = { GetRect().width, GetRect().height };
		}
	}

	void GameObject::LoadState(const std::shared_ptr<ISave> save)
	{
		if (auto objectSave = std::dynamic_pointer_cast<GameObjectSave>(save))
		{
			SetScaleBySize(sprite, objectSave->size);
			SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
			sprite.setPosition(objectSave->position);
		}
	}

	void GameObjectSave::SaveToFile(std::ofstream& ostream) const
	{
		ostream << position.x << " " << position.y << " " << size.x << " " << size.y << " ";
	}

	void GameObjectSave::LoadFromFile(std::ifstream& istream)
	{
		istream >> position.x >> position.y >> size.x >> size.y;
	}
		
}