#include "BaseInputHandler.h"
#include "Settings.h"
#include "Game.h"

namespace SnakeGame
{
	void BaseInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{	
		Settings* settings = Settings::GetSettings();
		Game* game = Game::GetGame();
		for (auto& inputEvent : input)
		{
			if (settings->keyMap.contains(inputEvent.key.code))
			{
				if (actionMapping.contains(settings->keyMap[inputEvent.key.code]))
				{
					game->PlaySound(SoundType::OnKeyHit);
					actionMapping.at(settings->keyMap[inputEvent.key.code]) (this);
				}
			}
		}		
	}
}