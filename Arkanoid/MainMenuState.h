#pragma once
#include "BaseState.h"
#include "MainMenu.h"

namespace Arkanoid
{
	class MainMenuState : public BaseState
	{
	public:
		MainMenuState();
		virtual ~MainMenuState() = default;
		virtual void Draw(sf::RenderWindow& window) const override;
	private:
		MainMenu menu;
	};

}
