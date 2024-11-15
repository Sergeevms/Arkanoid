#include "BaseInputHandler.h"
#include "Application.h"
#include "GameWorld.h"
#include "Game.h"

namespace Arkanoid
{
	void BaseInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{	
		GameWorld* world = GameWorld::GetWorld();
		Game* game = Application::GetInstance().GetGame();
		for (auto& inputEvent : input)
		{
			if (world->keyMap.contains(inputEvent.key.code))
			{
				if (actionMapping.contains(world->keyMap[inputEvent.key.code]))
				{
					game->PlaySound(SoundType::OnKeyHit);
					actionMapping.at(world->keyMap[inputEvent.key.code]) (this);
				}
			}
		}		
	}
}