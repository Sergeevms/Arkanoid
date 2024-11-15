#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	enum class ActionsTypesOnInput;
	enum class BlockType;

	class GameWorld
	{
	public:
		static GameWorld* GetWorld()
		{
			static GameWorld world;
			return &world;
		};
		sf::Vector2f ScreenCenter();
		sf::Vector2f ScreenSize();

		//General world

		int screenWidth;
		int screenHeight;
		std::wstring gameName;
		std::wstring defaultPlayerName;
		float popUpSpacing;
		float popUpBorder;
		float timePerFrame;
		float sessionDelayTime;
		int smallRecordsSize;
		int bigRecordsSize;

		sf::Color halfTrasparentWhite;

		//Block world
		int blocksInRow;
		int blockRowCount;
		float blockSpacing;
		sf::Vector2f blockSize;
		std::vector<BlockType> availiableBlockTypes;
		std::unordered_map<BlockType, sf::Color> blockColors;
		float smoothBlockDestroyTime;
		int multiHitBlockCount;

		//Sound world

		bool musicOn;
		bool soundOn;		
				
		//Paths to files

		std::string resourcesPath;
		std::string soundPath;
		std::string fontPath;
		std::string texturePath;

		std::string recordsFileName;
		
		//Control world
		std::unordered_map<sf::Keyboard::Key, ActionsTypesOnInput> keyMap;

		//Platform world

		sf::Vector2f platformSize;
		float platformSpeed;

		//Ball world

		float ballDiameter;
		float ballSpeed;
		float angleRandomChange;
		float anglePlatformReboundChange;
	private:
		GameWorld();
		~GameWorld() = default;
		GameWorld(const GameWorld&) = delete;
		GameWorld operator= (const GameWorld&) = delete;
	};
};