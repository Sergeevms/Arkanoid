#include "MainMenuInputHandler.h"
#include "MainMenu.h"
#include "Settings.h"
#include "Game.h"
#include "BaseState.h"

namespace Arkanoid
{
	MainMenuInputHandler::MainMenuInputHandler(MainMenu* currentMenu) : BaseMenuInputHandler(currentMenu), menu(currentMenu)
	{
		activateMapping[MenuNodeActivateReaction::Exit] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->ExitGame(); }};
		activateMapping[MenuNodeActivateReaction::Play] = [this](BaseInputHandler* handler)
			{if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->StartPlaying(); }};
		activateMapping[MenuNodeActivateReaction::SwitchOption] = [this](BaseInputHandler* handler)
			{ if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->SwitchOption(); }};
		activateMapping[MenuNodeActivateReaction::Records] = [this](BaseInputHandler* handler)
			{ if (auto currentHandler = dynamic_cast<MainMenuInputHandler*>(this)) { currentHandler->ShowRecords(); } }; 
	}

	void MainMenuInputHandler::ExitGame()
	{
		Game::GetGame()->ShutDown();
	}

	void MainMenuInputHandler::StartPlaying()
	{
		Game::GetGame()->SwitchToState(GameState::Playing);
	}

	void MainMenuInputHandler::ShowRecords()
	{
		Game::GetGame()->SwitchToState(GameState::Records);
	}

	void MainMenuInputHandler::SwitchOption()
	{
		menu->SwitchChecked();
	}
}