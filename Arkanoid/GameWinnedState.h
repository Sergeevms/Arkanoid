#pragma once
#include <SFML/Graphics.hpp>
#include "BaseState.h"

namespace Arkanoid
{
	class GameWinnedMenu;

	class GameWinnedState : public BaseState
	{
	public:
		GameWinnedState();
		virtual void Draw(sf::RenderWindow& window) const override;
	private:
		sf::RectangleShape background;
		std::unique_ptr<GameWinnedMenu> gameOverMenu;
	};
}
