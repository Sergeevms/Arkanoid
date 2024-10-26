#pragma once
#include "GeneralMenu.h"
namespace Arkanoid
{
	class RecordsStateNameMenu : public GeneralMenu
	{
	public:
		RecordsStateNameMenu();
		virtual ~RecordsStateNameMenu() = default;
	};

	class RecordsStateMenu : public GeneralMenu
	{
	public:
		RecordsStateMenu();
		virtual ~RecordsStateMenu() = default;
		void Draw(sf::RenderWindow& window, const sf::Vector2f& position, RelativePosition origin) const override;
	};
}

