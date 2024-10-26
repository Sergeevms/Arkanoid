#pragma once
#include "BaseMenuInputHandler.h"
#include <unordered_map>


namespace Arkanoid
{
    class PauseMenu;

	class PauseStateInputHandler : public BaseMenuInputHandler
	{
    public:
        PauseStateInputHandler(PauseMenu* currentMenu);
    private:
        void ReturnToGame();
        void ReturnToMenu();
	};
}

