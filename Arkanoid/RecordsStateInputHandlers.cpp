#include "RecordsState.h"
#include "RecordsStateMenus.h"
#include "RecordsStateInputHandlers.h"
#include "Application.h"

namespace Arkanoid
{
	RecordsStateNameEnteringInputHandler::RecordsStateNameEnteringInputHandler(sf::Text* nameText) :
		BaseInputHandler(), name(nameText)
	{
		actionMapping[ActionsTypesOnInput::BackSpace] = 
			[](BaseInputHandler* handler)
			{
				if (auto currentHandler = dynamic_cast<RecordsStateNameEnteringInputHandler*>(handler))
				{
					currentHandler->RemoveSymbol();
				}
			};

		actionMapping[ActionsTypesOnInput::Forward] = 
			[](BaseInputHandler* handler)
			{
				if (auto state = dynamic_cast<RecordsState*> (Application::GetInstance().GetGame()->GetState()))
				{
					state->SwitchToWindow(RecordStateWindowType::RecordTable);
				}
			};
	}

	void RecordsStateNameEnteringInputHandler::HandleInputEvents(const std::vector<sf::Event>& input)
	{
		for (auto& inputEvent : input)
		{
			switch (inputEvent.type)
			{
			case (sf::Event::TextEntered):
			{
				if (inputEvent.text.unicode >= 0x20)
				{			
					name->setString(name->getString() + sf::String(inputEvent.text.unicode));
				}
				break;
			}
			case (sf::Event::KeyPressed):
			{
				Settings* settings = Application::GetSettings();
				if (settings->keyMap.contains(inputEvent.key.code))
				{
					if (actionMapping.contains(settings->keyMap[inputEvent.key.code]))
					{
						Application::GetInstance().GetGame()->PlaySound(SoundType::OnKeyHit);
						actionMapping.at(settings->keyMap[inputEvent.key.code]) (this);
					}
				}
				break;
			}
			}
		}
	}

	void RecordsStateNameEnteringInputHandler::RemoveSymbol()
	{		
		sf::String newName = name->getString();
		if (newName.getSize() > 0)
		{
			newName.erase(newName.getSize() - 1, 1);
		}
		name->setString(newName);
	}

	RecordsStateMenuInputHandler::RecordsStateMenuInputHandler()
	{
		actionMapping[ActionsTypesOnInput::Back] =
			[](BaseInputHandler*)
			{
				Application::GetInstance().GetGame()->SwitchToState(GameState::MainMenu);
			};
	}
}
