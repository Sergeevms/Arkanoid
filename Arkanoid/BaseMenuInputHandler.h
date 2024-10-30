#pragma once
#include "BaseInputHandler.h"
namespace Arkanoid
{
    class GeneralMenu;

    /*Extended BasedInputHandler with tipical actions for menu states.
    * Added map for reacting on menu nodes that needs some actions
    */
    class BaseMenuInputHandler : public BaseInputHandler
    {
    public:
        BaseMenuInputHandler(GeneralMenu* currentMenu);
        virtual ~BaseMenuInputHandler() = default;
        virtual GeneralMenu* GetMenu();
    protected:
        GeneralMenu* menu;
    };
}

