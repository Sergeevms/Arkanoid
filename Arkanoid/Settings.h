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
		
		sf::Vector2f ScreenCenter();
		sf::Vector2f ScreenSize();

		//General settings

		int screenWidth;
		int screenHeight;
		std::wstring gameName;
		std::wstring defaultPlayerName;
		float popUpSpacing;
		float popUpBorder;
		int smallRecordsSize;
		int bigRecordsSize;

		//Sound settings

		bool musicOn;
		bool soundOn;		
				
		//Paths to files

		std::string resourcesPath;
		std::wstring levelPath;
		std::string soundPath;
		std::string fontPath;
		std::string texturePath;

		std::string recordsFileName;
		
		//Control settings
		std::unordered_map<sf::Keyboard::Key, ActionsTypesOnInput> keyMap;
	private:
		Settings();
		static Settings* settings;
	};
};