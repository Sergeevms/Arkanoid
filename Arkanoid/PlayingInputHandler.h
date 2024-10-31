#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "BaseInputHandler.h"

namespace Arkanoid
{
	class Platform;
	enum class Direction;

	class PlayingInputHandler : public BaseInputHandler
	{
	public:
		PlayingInputHandler(Platform* platform);
		virtual void HandleInputEvents(const std::vector<sf::Event>& input) override;
	private:
		Platform* platform;
	};
}