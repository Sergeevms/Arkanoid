#include <assert.h>
#include <fstream>

#include "Map.h"
#include "MapObject.h"
#include "Utility.h"
#include "Wall.h"
#include "Apple.h"
#include "Snake.h"
#include "Settings.h"

namespace SnakeGame
{
	Map::Map()
	{
		LoadTexture("Wall.png", wallTexture);
	}

	void Map::LoadFromFile(const std::wstring& fileName)
	{
		std::ifstream input;
		Settings* settings = Settings::GetSettings();
		input.open(settings->levelPath + fileName);
		if (input.is_open())
		{
			input >> width >> height;
			charMap.clear();
			charMap.reserve(height);
			std::string currentString;
			for (int i = 0; i < height; ++i)
			{
				input >> currentString;
				charMap.push_back(currentString);
			}
			input.close();
		}
	}

	void Map::CreateSavedLvl()
	{
		if (!map.empty())
		{
			map.clear();
		}

		emptyCellCount = width * height;
		map.reserve(width * height);

		bool applePlaced = false;

		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				sf::Vector2i currentCell{ i, j };
				std::shared_ptr<MapObject> currentObject{ nullptr };
				switch (charMap[j].at(i))
				{
				case 'W':
				{
					currentObject = std::make_shared<Wall>(currentCell, wallTexture, getWallDirection(currentCell));
					break;
				}
				case 'T':
				{
					temporaryWalls.push_back(std::make_shared<TemporaryWall>(currentCell, wallTexture, getWallDirection(currentCell)));
					currentObject = temporaryWalls.back();
					break;
				}
				case 'A':
				{
					loadedApplePosition = currentCell;
					break;
				}
				case 'H':
				{
					loadedSnakeHeadPosition = currentCell;
					break;
				}
				}
				
				map.emplace_back(currentObject);
				if (IsCollisionOveringGame(currentObject.get()))
				{
					--emptyCellCount;
				}
			}
		}
	}

	void Map::Draw(sf::RenderWindow& window) const
	{		
		if (temporaryWallsEmplaced == false)
		{
			for (auto& wall : temporaryWalls)
			{
				wall->Draw(window);
			}
		}
		for (auto& object : map)
		{
			if (object)
			{
				object->Draw(window);
			}
		}			
	}

	void Map::EmplaceMapObject(std::shared_ptr<MapObject> object)
	{
		sf::Vector2i objectCell = object->GetCellPosition();
		map[CellToMapIndex(objectCell)] = object;

		if (IsCollisionOveringGame(object.get()))
		{
			--emptyCellCount;
		}
	}

	int Map::GetEmptyCellCount() const
	{
		return emptyCellCount;
	}

	void Map::RemoveMapObject(std::shared_ptr<MapObject> object)
	{
		sf::Vector2i objectCell = object->GetCellPosition();
		RemoveMapObject(objectCell);
	}

	void Map::RemoveMapObject(const sf::Vector2i& cell)
	{
		if (map[CellToMapIndex(cell)] &&
			(IsCollisionOveringGame(map[CellToMapIndex(cell)].get())))
		{
			++emptyCellCount;
		}
		map[CellToMapIndex(cell)] = nullptr;
	}

	MapObject* Map::GetObject(const sf::Vector2i& cell)
	{
		return map[CellToMapIndex(cell)].get();
	}

	sf::Vector2i Map::GetRandomEmptyCell() const
	{
		sf::Vector2i checkingCell{ rand() % width, rand() % height };
		while (map[CellToMapIndex(checkingCell)] != nullptr)
		{
			checkingCell = { rand() % width, rand() % height };
		}
		return checkingCell;
	}

	const sf::Vector2i& Map::GetLoadedSnakeHeadPosition() const
	{
		return loadedSnakeHeadPosition;
	}

	const sf::Vector2i& Map::GetLoadedApplePosition() const
	{
		return loadedApplePosition;
	}

	sf::Vector2i Map::GetMapSize() const
	{
		return sf::Vector2i{ width, height };
	}

	const std::vector<std::string>& Map::GetCharMap() const
	{
		return charMap;
	}

	bool Map::HaveEmptyCells() const
	{
		return emptyCellCount > 0;
	}

	bool Map::ValidCell(const sf::Vector2i& cell) const
	{
		return InRightOpenInterval(0, width, cell.x) && InRightOpenInterval(0, height, cell.y);
	}

	void Map::GenerateRandomWalls()
	{
		std::vector<int> availableMovingDirectionsFromCells(map.size(), 4);
		//Number of cells, adding wall to witch would'n block way throug nearby cells
		size_t availiableToAddWallCells = map.size();

		//Counting availiableToAddWallCells based on current map
		for (auto& object : map)
		{
			if (std::dynamic_pointer_cast<Wall>(object))
			{
				--availiableToAddWallCells;
				availableMovingDirectionsFromCells[CellToMapIndex(object->GetCellPosition())] = 0;
				for (auto& directionVector : directionVectorsI)
				{
					sf::Vector2i possibleCell = object->GetCellPosition() + directionVector.second;
					if (ValidCell(possibleCell) && availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)] > 0)
					{
						--availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)];
						if (availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)] == 2)
						{
							--availiableToAddWallCells;
						}
					}
				}
			}
			else if (std::dynamic_pointer_cast<MapObject>(object))
			{
				--availiableToAddWallCells;
			}
		}

		int targetRandomWallsCount = static_cast<int> (Settings::GetSettings()->randomWallCoefficient * availiableToAddWallCells);

		//Adding walls while target count is reached or availiable cells are gone
		while (targetRandomWallsCount > 0 && availiableToAddWallCells > 0)
		{
			sf::Vector2i newWallCell = GetRandomEmptyCell();
			bool goodCell = availableMovingDirectionsFromCells[CellToMapIndex(newWallCell)] > 0;
			//check that emplacing a wall wouldn't block nearby cells
			for (auto& directionVector : directionVectorsI)
			{
				sf::Vector2i nearCell = directionVector.second + newWallCell;
				if (ValidCell(nearCell) && (availableMovingDirectionsFromCells[CellToMapIndex(nearCell)] > 2 || availableMovingDirectionsFromCells[CellToMapIndex(nearCell)] == 0))
				{
					goodCell = goodCell && true;
				}
				else
				{
					goodCell = goodCell && false;
				}
			}
			//add wall is cell is acceptable; mark as unacceptable and decrease availiableToAddWallCells otherwise
			if (goodCell)
			{
				--availiableToAddWallCells;
				--targetRandomWallsCount;
				EmplaceMapObject(std::make_shared<Wall>(newWallCell, wallTexture, getWallDirection(newWallCell)));
				availableMovingDirectionsFromCells[CellToMapIndex(newWallCell)] = 0;
				for (auto& directionVector : directionVectorsI)
				{
					sf::Vector2i possibleCell = newWallCell + directionVector.second;
					if (ValidCell(possibleCell) && availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)] > 0)
					{
						--availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)];
						if (availableMovingDirectionsFromCells[CellToMapIndex(possibleCell)] == 2)
						{
							--availiableToAddWallCells;
						}
					}
				}
			}
			else
			{
				--availiableToAddWallCells;
				availableMovingDirectionsFromCells[CellToMapIndex(newWallCell)] = 0;
			}
		}
	}

	void Map::SetTemporaryWallsOpacity(const int opacity)
	{
		for (auto& wall : temporaryWalls)
		{
			wall->setOpacity(opacity);
		}
	}

	void Map::EmplaceTemporaryWalls()
	{
		temporaryWallsEmplaced = true;
		for (auto& wall : temporaryWalls)
		{
			if (GetObject(wall->GetCellPosition()) == nullptr)
			{
				EmplaceMapObject(wall);
			}
		}
	}

	void Map::RemoveTemporaryWalls()
	{
		temporaryWallsEmplaced = false;
		for (auto& wall : temporaryWalls)
		{
			if (GetObject(wall->GetCellPosition()) == wall.get())
			{
				RemoveMapObject(wall);
			}
		}
	}

	int Map::CellToMapIndex(const sf::Vector2i& cell) const
	{
		return cell.y * width + cell.x;
	}

	Direction Map::getWallDirection(const sf::Vector2i & cell) const
	{
		if (cell.x > 0 && (charMap[cell.y].at(cell.x - 1) == 'W'))
		{
			return Direction::Up;
		}
		else
		{
			if (cell.x < width - 1 && charMap[cell.y].at(cell.x + 1) == 'W')
			{
				return Direction::Down;
			}
			else
			{
				if (cell.y > 0 && charMap[cell.y - 1].at(cell.x) == 'W')
				{
					return Direction::Left;
				}
				else
				{
					return Direction::Rigth;
				}
			}
		}
	}
}
