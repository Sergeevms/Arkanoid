#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "BaseMenuInputHandler.h"

namespace SnakeGame
{
    class MainMenu;
    
    class MainMenuInputHandler : public BaseMenuInputHandler
    {
    public:
        MainMenuInputHandler(MainMenu* currentMenu);
        void ExitGame();
        void StartPlaying();
        void ShowRecords();
        void SwitchOption();
    private:
        MainMenu* menu;
    };
}

