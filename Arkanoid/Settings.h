#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	enum class ActionsTypesOnInput;

	class Settings
	{
	public:
		Settings();

		sf::Vector2f ScreenCenter();
		sf::Vector2f ScreenSize();

		//General settings

		int screenWidth;
		int screenHeight;
		std::wstring gameName;
		std::wstring defaultPlayerName;
		float popUpSpacing;
		float popUpBorder;
		float timePerFrame;
		int smallRecordsSize;
		int bigRecordsSize;

		sf::Color halfTrasparentWhite;

		//Sound settings

		bool musicOn;
		bool soundOn;		
				
		//Paths to files

		std::string resourcesPath;
		std::string soundPath;
		std::string fontPath;
		std::string texturePath;

		std::string recordsFileName;
		
		//Control settings
		std::unordered_map<sf::Keyboard::Key, ActionsTypesOnInput> keyMap;

		//Platform settings

		sf::Vector2f platformSize;
		float platformSpeed;

		//Ball settings

		float ballDiameter;
		float ballSpeed;
	};
};