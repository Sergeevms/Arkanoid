#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
	enum class Direction;
	class MapObject;
	class TemporaryWall;

	class Map
	{
	public:
		Map();
		//Loading map discription from file setted in Settings::selectedLevel
		void LoadFromFile(const std::wstring& fileName);
		//Creates level based on loaded discription
		void CreateSavedLvl();
		void Draw(sf::RenderWindow& window) const;
		void EmplaceMapObject(std::shared_ptr <MapObject> object);
		//Returns count of cells which colide result woudn't results in Game over
		int GetEmptyCellCount() const;
		void RemoveMapObject(std::shared_ptr<MapObject> object);
		void RemoveMapObject(const sf::Vector2i& cell);
		MapObject* GetObject(const sf::Vector2i& cell);
		/*Returns coordinates of non-occupied cell*/
		sf::Vector2i GetRandomEmptyCell() const;
		//Returns cell coordinate of snake head in loaded map sicription, {-1, -1} otherwise
		const sf::Vector2i& GetLoadedSnakeHeadPosition() const;
		//Returns cell coordinate of apple in loaded map sicription, {-1, -1} otherwise
		const sf::Vector2i& GetLoadedApplePosition() const;
		sf::Vector2i GetMapSize() const;
		const std::vector<std::string>& GetCharMap() const;
		bool HaveEmptyCells() const;
		/*Check that cell in borders of loaded map*/
		bool ValidCell(const sf::Vector2i& cell) const;
		void GenerateRandomWalls();
		void SetTemporaryWallsOpacity(const int opacity);
		void EmplaceTemporaryWalls();
		void RemoveTemporaryWalls();
	private:
		std::vector <std::shared_ptr<MapObject>> map;
		std::vector <std::string> charMap;
		sf::Vector2i loadedSnakeHeadPosition{ -1, -1 };
		sf::Vector2i loadedApplePosition{ -1, -1 };
		sf::Texture wallTexture;
		bool temporaryWallsEmplaced{ true };
		int CellToMapIndex(const sf::Vector2i& cell) const;
		int width{ 0 };
		int height{ 0 };
		int emptyCellCount{ 0 };
		std::vector <std::shared_ptr<TemporaryWall>> temporaryWalls;
		Direction getWallDirection(const sf::Vector2i & cell) const;
	};
}
