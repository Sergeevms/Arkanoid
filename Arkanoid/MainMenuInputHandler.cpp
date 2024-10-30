#include "MainMenuInputHandler.h"
#include "Application.h"
#include "MainMenu.h"
#include "Settings.h"
#include "Game.h"
#include "BaseState.h"

namespace Arkanoid
{
	MainMenuInputHandler::MainMenuInputHandler(MainMenu* currentMenu) : BaseMenuInputHandler(currentMenu), menu(currentMenu)
	{
		/*activateMapping[MenuNodeActivateReaction::Exit] = [](BaseInputHandler* handler)
			{ Application::GetInstance().GetGame()->ShutDown(); };
		activateMapping[MenuNodeActivateReaction::Play] = [this](BaseInputHandler* handler)
			{ Application::GetInstance().GetGame()->SwitchToState(GameState::Playing); };
		activateMapping[MenuNodeActivateReaction::SwitchOption] = [this](BaseInputHandler* handler)
			{ if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->SwitchOption(); }};
		activateMapping[MenuNodeActivateReaction::Records] = [this](BaseInputHandler* handler)
			{ if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->ShowRecords(); } }; */
	}

	/*void MainMenuInputHandler::ExitGame()
	{
		Application::GetInstance().GetGame()->ShutDown();
	}

	void MainMenuInputHandler::StartPlaying()
	{
		Application::GetInstance().GetGame()->SwitchToState(GameState::Playing);
	}

	void MainMenuInputHandler::ShowRecords()
	{
		Application::GetInstance().GetGame()->SwitchToState(GameState::Records);
	}

	void MainMenuInputHandler::SwitchOption()
	{
		menu->SwitchChecked();
	}*/
}