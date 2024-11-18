#include "LevelLoader.h"
#include <fstream>
#include "Block.h"
#include "GameWorld.h"

namespace Arkanoid
{
	LevelLoader::LevelLoader()
	{
		LoadLevelsFromFile();
	}

	Level& LevelLoader::GetLevel(int i)
	{
		return levels[i];
	}

	int LevelLoader::GetLevelCount()
	{
		return static_cast<int>(levels.size());
	}

	void LevelLoader::LoadLevelsFromFile()
	{
		levels.clear();
		std::ifstream input(GameWorld::GetWorld()->levelConfig);
		if (input.is_open())
		{
			std::string line;
			int y = 0;
			while (getline(input, line))
			{
				if (line.rfind("level ", 0) == 0)
				{
					auto level = std::stoi(line.substr(6, line.size() - 6));
					levels.emplace_back(Level());
					y = 0;
				}
				else
				{
					int x = 0;
					for (char c : line)
					{
						if (c != ' ')
						{
							levels.back().blocks.emplace_back(std::make_pair(sf::Vector2i{ x, y }, CharToBlockType(c)));
							++x;
						}
					}
					++y;
				}
			}
		}
	}

	BlockType LevelLoader::CharToBlockType(char c)
	{
		BlockType blockType;
		switch (c)
		{
		case '1':
			blockType = BlockType::Simple;
			break;
		case '0':
			blockType = BlockType::Unbreackble;
			break;
		case '2':
			blockType = BlockType::MultiHit;
			break;
		case '3':
			blockType = BlockType::Glass;
			break;
		case 'R':
			blockType = GameWorld::GetWorld()->availiableBlockTypes[rand() % GameWorld::GetWorld()->availiableBlockTypes.size()];
			break;
		default:
			assert(false);
			break;
		}
		return blockType;
	}
}