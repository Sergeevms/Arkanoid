#include "BaseMenuInputHandler.h"
#include "GeneralMenu.h"
#include "Application.h"

namespace Arkanoid
{
	BaseMenuInputHandler::BaseMenuInputHandler(GeneralMenu* currentMenu) : 
		BaseInputHandler(), menu(currentMenu)
	{
		actionMapping[ActionsTypesOnInput::Up] = 
			[](BaseInputHandler* handler)
			{
				if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(handler)) 
				{ 
					if (currentHandler->GetMenu()) 
						currentHandler->GetMenu()->SelectPrevious();
				}
			};

		actionMapping[ActionsTypesOnInput::Down] = 
			[](BaseInputHandler* handler)
			{
				if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(handler))
				{
					if (currentHandler->GetMenu())
						currentHandler->GetMenu()->SelectNext(); 
				}
			};

		actionMapping[ActionsTypesOnInput::Forward] = 
			[](BaseInputHandler* handler)
			{
				if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(handler)) 
				{ 
					if (currentHandler->GetMenu())
						currentHandler->GetMenu()->PressSelected(); 
				}
			};

		actionMapping[ActionsTypesOnInput::Back] = 
			[](BaseInputHandler* handler)
			{
				if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(handler)) 
				{ 
					if (currentHandler->GetMenu())
						currentHandler->GetMenu()->ReturnToPrevious(); 
				}
			};
	}

	GeneralMenu* BaseMenuInputHandler::GetMenu()
	{
		return menu;
	}
}