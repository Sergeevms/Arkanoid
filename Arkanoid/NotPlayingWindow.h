#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace SnakeGame
{
	enum class RelativePosition;
	class BaseInputHandler;
	class BaseState;

	class NotPlayingWindow
	{
	public:
		virtual ~NotPlayingWindow() = default;
		virtual void Draw(sf::RenderWindow& window) = 0;
		virtual void HandleInput(const std::vector<sf::Event>& input);
	protected:
		std::unique_ptr<BaseInputHandler> inputHandler;
		BaseState* state = nullptr;
	};
}

