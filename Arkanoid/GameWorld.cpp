#include "GameWorld.h"
#include <filesystem>
#include "BaseInputHandler.h"
#include "Utility.h"
#include "Block.h"
#include "Bonus.h"

namespace Arkanoid
{
	GameWorld::GameWorld()
	{
		resourcesPath = "Resources\\";
		soundPath = resourcesPath + "Sounds\\";
		fontPath = resourcesPath + "Fonts\\";
		texturePath = resourcesPath + "Textures\\";
		levelConfig = resourcesPath + "winTestlevels.cnfg";
		//levelConfig = resourcesPath + "levels.cnfg";
		saveFile = "save.dat";

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

		blocksInRow = 15;
		blockRowCount = 3;
		blockSpacing = 3.f;
		blockSize.y = 30.f;
		blockSize.x = (screenWidth - (1 + blocksInRow) * blockSpacing) / blocksInRow;
		multiHitBlockCount = 3;
		availiableBlockTypes.push_back(BlockType::SmoothDestroyable);
		availiableBlockTypes.push_back(BlockType::Unbreackble);
		availiableBlockTypes.push_back(BlockType::MultiHit);
		availiableBlockTypes.push_back(BlockType::Glass);
		blockColors[BlockType::Unbreackble] = sf::Color(132, 139, 148, 255);
		blockColors[BlockType::Simple] = sf::Color::White;
		blockColors[BlockType::SmoothDestroyable] = sf::Color::Red;
		blockColors[BlockType::MultiHit] = sf::Color(154, 73, 209, 255);
		blockColors[BlockType::Glass] = sf::Color::Cyan;
		blockScore[BlockType::Unbreackble] = 20;
		blockScore[BlockType::Simple] = 1;
		blockScore[BlockType::SmoothDestroyable] = 2;
		blockScore[BlockType::MultiHit] = blockScore[BlockType::SmoothDestroyable] * multiHitBlockCount;
		blockScore[BlockType::Glass] = 3;
		mapCharToBlockTypeMapping['0'] = BlockType::Unbreackble;
		mapCharToBlockTypeMapping['1'] = BlockType::SmoothDestroyable;
		mapCharToBlockTypeMapping['2'] = BlockType::MultiHit;
		mapCharToBlockTypeMapping['3'] = BlockType::Glass;
		mapCharToBlockTypeMapping['N'] = BlockType::Simple;
		smoothBlockDestroyTime = 1.5f;

		bonusSize = 20.f;
		bonusSpeed = 175.f;
		bonusDuration = 10.f;
		
		avaliableBonuses.push_back(BonusType::ballSpeed);
		avaliableBonuses.push_back(BonusType::platformSize);
		avaliableBonuses.push_back(BonusType::oneHitBlock);

		bonusAppearanceChance = 10.f;
		bonusColors[BonusType::platformSize] = sf::Color::Green;
		bonusColors[BonusType::ballSpeed] = sf::Color::Red;
		bonusColors[BonusType::oneHitBlock] = sf::Color::Yellow;
		platformBonusFactor = 1.5f;
		ballSpeedBonusFactor = 1.2f;
		acceleratedBallColor = sf::Color::Red;
		oneHitBlockColor = blockColors[BlockType::Glass] * blockColors[BlockType::Unbreackble];
		oneHitBlockColor.a = 100;
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
