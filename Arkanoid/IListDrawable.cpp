#include <assert.h>
#include "IListDrawable.h"
#include "Utility.h"
#include "GameWorld.h"

namespace Arkanoid
{

	sf::FloatRect GetListRect(const std::vector<IListDrawable*> itemList, const sf::Vector2f position, 
		const RelativePosition origin, const Orientation orientation, const Alignment alignment, const float spacing = 0.f)
	{
		sf::FloatRect totalRect;		

		float maxItemSize = 0;
		int subListCount = 0;

		for (auto& currentItem : itemList)
		{
			sf::FloatRect currentItemRect = currentItem->GetRect();
			if (orientation == Orientation::Vertical)
			{
				totalRect.width = std::max(totalRect.width, currentItemRect.width);
				if (dynamic_cast<DrawableList*>(currentItem))
				{
					totalRect.height += currentItemRect.height;
					++subListCount;
				}
				else
				{
					maxItemSize = std::max(maxItemSize, currentItemRect.height);
				}
			}
			else
			{
				totalRect.height = std::max(totalRect.height, currentItemRect.height);
				if (dynamic_cast<DrawableList*>(currentItem))
				{
					totalRect.width += currentItemRect.width;
					++subListCount;
				}
				else
				{
					maxItemSize = std::max(maxItemSize, currentItemRect.width);
				}
			}
		}

		if (orientation == Orientation::Vertical)
		{
			totalRect.height += maxItemSize * (itemList.size() - subListCount) + spacing * (itemList.size() - 1);
		}
		else
		{
			totalRect.width += maxItemSize * (itemList.size() - subListCount) + spacing * (itemList.size() - 1);
		}
		
		totalRect.top = position.y - relativePositions.at(origin).y * totalRect.height;
		totalRect.left = position.x - relativePositions.at(origin).x * totalRect.width;

		return totalRect;
	}

	void DrawList(sf::RenderWindow& window, const std::vector<IListDrawable*> itemList, const sf::Vector2f position,
		const RelativePosition origin, const Orientation orientation, const Alignment alignment, const float spacing = 0.f)
	{
		sf::FloatRect totalRect = GetListRect(itemList, position, origin, orientation, alignment, spacing);
		sf::Vector2f relativeOrigin = relativePositions.at(RelativePositionByOrientationAndAlignment(orientation, alignment));
		sf::Vector2f currentPosition;
		currentPosition.x = totalRect.left + (orientation == Orientation::Vertical ? totalRect.width * relativeOrigin.x : 0.f);
		currentPosition.y = totalRect.top + (orientation == Orientation::Vertical ? 0.f : totalRect.height * relativeOrigin.y);

		for (auto& currentItem : itemList)
		{
			sf::FloatRect currentRect = currentItem->GetRect();
			currentItem->Draw(window, currentPosition, orientation, alignment);
			if (orientation == Orientation::Vertical)
			{
				currentPosition.y += currentRect.height + spacing;
			}
			else
			{
				currentPosition.x += currentRect.width + spacing;
			}
		}
	}

	void CreateListDrawableTextInVector(std::vector<ListDrawableText>& texts, const TextStyle* newStyle, std::wstring text)
	{
		texts.emplace_back(ListDrawableText());
		texts.back().SetStyle(newStyle);
		texts.back().setString(text);
	}

	sf::FloatRect ListDrawableText::GetRect() const
	{
		sf::FloatRect rect = getLocalBounds();
		rect.width += rect.left;
		rect.height += rect.top;
		rect.left = 0.f;
		rect.top = 0.f;
		return rect;
	}

	void ListDrawableText::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{		
		PresetPosition(position, orientation, alignment);
		Draw(window);
	}

	void ListDrawableText::PresetPosition(const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{
		PresetPosition(position, RelativePositionByOrientationAndAlignment(orientation, alignment));
	}

	void ListDrawableText::PresetPosition(const sf::Vector2f& position, RelativePosition relativeOrigin)
	{
		sf::FloatRect textRect = getLocalBounds();
		sf::Vector2f correctPosition{ position.x - textRect.left, position.y - textRect.top };
		SetOriginByRelative(*this, relativePositions.at(relativeOrigin));
		setPosition(correctPosition);
	}

	void ListDrawableText::Draw(sf::RenderWindow& window) const
	{
		window.draw(*this);
	}

	void ListDrawableText::SetStyle(const TextStyle* newStyle)
	{
		setFont(newStyle->font);
		setCharacterSize(newStyle->characterSize);
		setStyle(newStyle->textStyle);
		setFillColor(newStyle->color);
	}

	DrawableList::DrawableList(const Orientation orientation, const Alignment alignment, const float spacing)
	{
		DrawableList::orientation = orientation;
		DrawableList::alignment = alignment;
		DrawableList::spacing = spacing;
	}

	sf::FloatRect DrawableList::GetRect() const
	{
		return GetListRect(itemList, { 0.f, 0.f }, RelativePosition::TopLeft, orientation, alignment, spacing);
	}

	void DrawableList::Draw(sf::RenderWindow& window, const sf::Vector2f& position, const Orientation orientation, const Alignment alignment)
	{
		DrawList(window, itemList, position, RelativePositionByOrientationAndAlignment(orientation, alignment), DrawableList::orientation, DrawableList::alignment, spacing);
	}

	void DrawableList::AddItem(IListDrawable* item)
	{
		itemList.push_back(item);
	}
}