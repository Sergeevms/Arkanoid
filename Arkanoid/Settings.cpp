#include "Settings.h"
#include <filesystem>
#include "BaseInputHandler.h"
#include "Utility.h"

namespace Arkanoid
{
	Settings::Settings()
	{
		resourcesPath = "Resources\\";
		soundPath = resourcesPath + "Sounds\\";
		fontPath = resourcesPath + "Fonts\\";
		texturePath = resourcesPath + "Textures\\";

		screenWidth = 800;
		screenHeight = 600;
		gameName = L"Arcanoid";
		defaultPlayerName = L"XYZ";
		popUpSpacing = 100.f;
		popUpBorder = 5.f;
		timePerFrame = 1.f / 120.f;
		musicOn = true;
		soundOn = true;

		halfTrasparentWhite = sf::Color::White;
		halfTrasparentWhite.a = 125;

		keyMap[sf::Keyboard::W] = ActionsTypesOnInput::Up;
		keyMap[sf::Keyboard::Up] = ActionsTypesOnInput::Up;
		keyMap[sf::Keyboard::D] = ActionsTypesOnInput::Right;
		keyMap[sf::Keyboard::Right] = ActionsTypesOnInput::Right;
		keyMap[sf::Keyboard::S] = ActionsTypesOnInput::Down;
		keyMap[sf::Keyboard::Down] = ActionsTypesOnInput::Down;
		keyMap[sf::Keyboard::A] = ActionsTypesOnInput::Left;
		keyMap[sf::Keyboard::Left] = ActionsTypesOnInput::Left;
		keyMap[sf::Keyboard::B] = ActionsTypesOnInput::Back;
		keyMap[sf::Keyboard::P] = ActionsTypesOnInput::Pause;
		keyMap[sf::Keyboard::Enter] = ActionsTypesOnInput::Forward;
		keyMap[sf::Keyboard::BackSpace] = ActionsTypesOnInput::BackSpace;

		keyMap[sf::Keyboard::R] = ActionsTypesOnInput::RecordTable;

		recordsFileName = "Records.dat";
		smallRecordsSize = 5;
		bigRecordsSize = 10;

		platformSize.x = 100.f;
		platformSize.y = 10.f;
		platformSpeed = 400.f;

		ballDiameter = 10.f;
		ballSpeed = 400.f;
	}


	sf::Vector2f Settings::ScreenCenter()
	{
		return ScreenSize() * 0.5f;
	}

	sf::Vector2f Settings::ScreenSize()
	{
		return sf::Vector2f({static_cast<float>(screenWidth), static_cast<float>(screenHeight)});
	}
}
