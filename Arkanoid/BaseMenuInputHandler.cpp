#include "BaseMenuInputHandler.h"
#include "GeneralMenu.h"

namespace SnakeGame
{
	BaseMenuInputHandler::BaseMenuInputHandler(GeneralMenu* currentMenu) : 
		BaseInputHandler(), menu(currentMenu)
	{
		actionMapping[ActionsTypesOnInput::Up] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(this)) { currentHandler->SelectPrevious(); }};
		actionMapping[ActionsTypesOnInput::Down] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(this)) { currentHandler->SelectNext(); }};
		actionMapping[ActionsTypesOnInput::Forward] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(this)) { currentHandler->ActivateCurrent(); }};
		actionMapping[ActionsTypesOnInput::Back] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<BaseMenuInputHandler*>(this)) { currentHandler->GoBack(); }};
	}

	void BaseMenuInputHandler::SelectNext()
	{
		menu->SelectNext();
	}

	void BaseMenuInputHandler::SelectPrevious()
	{
		menu->SelectPrevious();
	}

	void BaseMenuInputHandler::ActivateCurrent()
	{
		if (!menu->ExpandSelected())
		{
			activateMapping.at(menu->GetReaction()) (this);
		}
	}

	void BaseMenuInputHandler::GoBack()
	{
		menu->ReturnToPrevious();
	}
}