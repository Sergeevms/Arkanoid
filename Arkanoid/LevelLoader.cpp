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
		auto world = GameWorld::GetWorld();
		//Check if char is in mapping to blockType and availiable block types
		if (world->mapCharToBlockTypeMapping.contains(c))
		{
			blockType = world->mapCharToBlockTypeMapping.at(c);
			assert(std::find(world->availiableBlockTypes.begin(), world->availiableBlockTypes.end(), blockType) != world->availiableBlockTypes.end());
		}
		else
		//Check for special mapping (not direct char to block type
		{
			switch (c)
			{
			case 'R':
				blockType = GameWorld::GetWorld()->availiableBlockTypes[rand() % GameWorld::GetWorld()->availiableBlockTypes.size()];
				break;
			default:
				assert(false);
				break;
			}
		}
		return blockType;
	}
}