#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
	enum class ActionsTypesOnInput;

	struct Settings
	{
		static Settings* GetSettings()
		{
			if (settings == nullptr)
			{
				settings = new Settings();
			}
			return settings;
		};

		void LoadLevelsNames();
		sf::Vector2f ScreenCenter();
		sf::Vector2f ScreenSize();

		//General settings

		int screenWidth;
		int screenHeight;
		float tileSize;
		std::wstring gameName;
		std::wstring defaultPlayerName;
		float popUpSpacing;
		float popUpBorder;
		float movingDelayOnStart;
		int defaultSnakeSize;
		int smallRecordsSize;
		int bigRecordsSize;

		//Difficulty settings
		//Updates difficulty level and set new timeOnCell and movementSpeed according to it
		void UpdateDifficulty(const int newDiffculty);
		int GetCurrentDifficulty() const;
		float GetMovementSpeed() const;
		float GetTimeOnCell() const;
		std::unordered_map<int, float> difficultyToTimeOnCell;
		std::unordered_map<int, int> difficultyToScore;
		int difficultyLevelCount;
		int scoreOnDifficultyStep;
		float baseTimeOnCell;
		float timeOnCellDifficultyStep;
	private:
		int currentDifficulty;
		float movementSpeed;
		float timeOnCell;
	public:
		//Special apples settings
		//Chance to spawn special apple instead of usual in percents
		int specialAppleSpawnChance;
		bool disorientAppleOn;
		bool goldenAppleOn;
		bool poisionedAppleOn;
		float disorientationTime;
		float goldenAppleLifeTime;
		float goldenAppleScoreModifier;
		//Count of cell before posioned status fades off
		int poisonedCellCount;
		float poisonedSpeedModifire;
		sf::Color disorientAppleColor;
		sf::Color poisionedAppleColor;
		sf::Color goldenAppleColor;
		
		//Special walls settings
		//True if additional random walls will spawn
		bool randomWallsOn;
		//True if showing only levels with temporary walls
		bool temporaryWallsOn;
		//Coefficient of how much empty cells will be occupied with new walls
		float randomWallCoefficient;
		float temporaryWallLifeTime;
		int fadingBorderValue;
		float temporaryWallFadingTime;

		//Sound settings

		bool musicOn;
		bool soundOn;		
				
		//Paths to files

		std::string resourcesPath;
		std::wstring levelPath;
		std::string soundPath;
		std::string fontPath;
		std::string texturePath;

		std::wstring selectedLevel;
		std::vector<std::wstring> normalLevels;
		std::vector<std::wstring> temporaryWallsLevels;
		std::string recordsFileName;
		
		//Control settings
		std::unordered_map<sf::Keyboard::Key, ActionsTypesOnInput> keyMap;
	private:
		Settings();
		static Settings* settings;
	};
};