#include "GameWorld.h"
#include <filesystem>
#include "BaseInputHandler.h"
#include "Utility.h"
#include "Block.h"

namespace Arkanoid
{
	GameWorld::GameWorld()
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

		recordsFileName = "Records.dat";
		smallRecordsSize = 5;
		bigRecordsSize = 10;

		platformSize.x = 100.f;
		platformSize.y = 15.f;
		platformSpeed = 400.f;

		ballDiameter = 10.f;
		ballSpeed = 400.f;
		angleRandomChange = 5.f;
		anglePlatformReboundChange = 20.f;

		sessionDelayTime = 1.f;

		blocksInRow = 10;
		blockRowCount = 3;
		blockSpacing = 3.f;
		blockSize.y = 30.f;
		blockSize.x = (screenWidth - (1 + blocksInRow) * blockSpacing) / blocksInRow;
		availiableBlockTypes.push_back(BlockType::Simple);
		availiableBlockTypes.push_back(BlockType::SmoothDestroyable);
		availiableBlockTypes.push_back(BlockType::Unbreackble);
		availiableBlockTypes.push_back(BlockType::MultiHit);
		availiableBlockTypes.push_back(BlockType::Glass);
		blockColors[BlockType::Simple] = sf::Color::Red;
		blockColors[BlockType::Unbreackble] = sf::Color(132, 139, 148, 255);
		blockColors[BlockType::SmoothDestroyable] = sf::Color::Green;
		blockColors[BlockType::MultiHit] = sf::Color(154, 73, 209, 255);
		blockColors[BlockType::Glass] = sf::Color::Cyan;

		smoothBlockDestroyTime = 1.5f;
		multiHitBlockCount = 3;
	}


	sf::Vector2f GameWorld::ScreenCenter()
	{
		return ScreenSize() * 0.5f;
	}

	sf::Vector2f GameWorld::ScreenSize()
	{
		return sf::Vector2f({static_cast<float>(screenWidth), static_cast<float>(screenHeight)});
	}
}
