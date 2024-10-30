#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "BaseMenuInputHandler.h"

namespace Arkanoid
{	
	class RecordsStateNameEnteringInputHandler : public BaseInputHandler
	{
	public:
		RecordsStateNameEnteringInputHandler() = delete;
		RecordsStateNameEnteringInputHandler(sf::Text* nameText);
		virtual ~RecordsStateNameEnteringInputHandler() = default;
		virtual void HandleInputEvents(const std::vector<sf::Event>& input) override;
		void RemoveSymbol();
	private:
		sf::Text* name;
	};

	class RecordsStateMenuInputHandler : public BaseInputHandler
	{
	public:
		RecordsStateMenuInputHandler();
	};
}

