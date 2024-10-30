#include "BaseInputHandler.h"
#include "Application.h"
#include "Settings.h"
#include "Game.h"

namespace Arkanoid
{
	void BaseInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{	
		Settings* settings = Application::GetSettings();
		Game* game = Application::GetInstance().GetGame();
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