#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "BaseInputHandler.h"

namespace Arkanoid
{
	//Game state types
	enum class GameState
	{
		None,
		MainMenu,
		Playing,
		Records,
		Pause
	};

	//Base game state class
	class BaseState
	{
	public:
		virtual ~BaseState() = default;
		virtual void Draw(sf::RenderWindow&) const = 0;
		virtual void Update(const float deltaTime) {};
		virtual void HandleInput(const std::vector<sf::Event>&);
	protected:
		std::unique_ptr<BaseInputHandler> inputHandler;
	};
}