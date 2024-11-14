#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <concepts>

namespace Arkanoid
{
	enum class Direction
	{
		None, Left, Up, Rigth, Down
	};	

	enum class RelativePosition
	{
		TopLeft, TopMiddle, TopRight, MiddleLeft, Center, BottomLeft
	};

	const static std::unordered_map<RelativePosition, sf::Vector2f> relativePositions
	{
		{RelativePosition::TopLeft, { 0.f, 0.f }},
		{RelativePosition::TopMiddle, { 0.5f, 0.f }},
		{RelativePosition::TopRight, { 1.f, 0.f }},
		{RelativePosition::MiddleLeft, { 0.f, 0.5f }},
		{RelativePosition::Center, { 0.5f, 0.5f }},
		{RelativePosition::BottomLeft, { 0.f, 1.f }}
	};

	enum class Orientation
	{
		Horizontal, Vertical
	};

	enum class Alignment
	{
		Min, Middle, Max
	};
		
	struct TextStyle
	{
		sf::Font font;
		sf::Color color{ sf::Color::White };
		sf::Text::Style textStyle{ sf::Text::Style::Regular };
		unsigned int characterSize{ 20 };
		void Init(const std::string fontName, const sf::Color newColor = sf::Color::White, const sf::Text::Style newTextStyle = sf::Text::Style::Regular, const unsigned int newSize = 20);
	};

	/*Concept for function to set origin by RelativeOrigin*/
	template<typename T>
	concept haveMethodsForRelativeOriginSetGet = requires(T & a, float x)
	{
		{ a.getLocalBounds() }-> std::same_as<sf::FloatRect>;
		a.setOrigin(x, x);
	};

	/*Function to set origin by RelativeOrigin*/
	template<typename T>
	void SetOriginByRelative(T& object, sf::Vector2f const& relativeOrigin)
		requires haveMethodsForRelativeOriginSetGet<T>
	{
		const sf::FloatRect objectBoundRect = object.getLocalBounds();
		object.setOrigin(objectBoundRect.width * relativeOrigin.x, objectBoundRect.height * relativeOrigin.y);
	}

	/*Concept for function to set object scale to reach desired size*/
	template<typename T>
	concept haveMethodsForSettingScaleBySize = requires(T & a, float x)
	{
		{ a.getLocalBounds() }-> std::same_as<sf::FloatRect>;
		a.setScale(x, x);
	};

	/*Funtion to set scale to reach desired size*/
	template<typename T>
	void SetScaleBySize(T& object, sf::Vector2f const& desiredSize)
		requires haveMethodsForSettingScaleBySize<T>
	{
		const sf::FloatRect objectBoundRect = object.getLocalBounds();
		const float xScale = desiredSize.x / objectBoundRect.width;
		const float yScale = desiredSize.y / objectBoundRect.height;
		object.setScale(xScale, yScale);
	}

	void LoadTexture(std::string const& fileName, sf::Texture& texture);

	/*Function to check than valueToCheck is in right-open interval [begin, end)
	 *return true if begin <= valueToCheck and valueToCheck < end*/
	template<typename T>
	bool InRightOpenInterval(T begin, T end, T valueToCheck)
		requires std::totally_ordered<T>
	{
		return begin <= valueToCheck and valueToCheck < end;
	}

	RelativePosition RelativePositionByOrientationAndAlignment(const Orientation orientation, const Alignment alignment);

	sf::Vector2f DirectionVecFromDegree(const float angleDegree);
	void ChangeSpriteOpacity(sf::Sprite& sprite, const sf::Uint8 alpha);
}