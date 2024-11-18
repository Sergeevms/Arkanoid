#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
namespace Arkanoid
{
	enum class BlockType;

	struct Level
	{
		std::vector<std::pair<sf::Vector2i, BlockType>> blocks;
	};

	class LevelLoader
	{
	public:
		LevelLoader();
		Level& GetLevel(int i);
		~LevelLoader() = default;
		int GetLevelCount();		
	private:
		void LoadLevelsFromFile();
		static BlockType CharToBlockType(char c);
		std::vector<Level> levels;
	};
}

