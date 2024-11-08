#pragma once
#include "BaseState.h"

namespace Arkanoid
{
    class GameObject;

    class PlayingState :
        public BaseState
    {
    public:
        PlayingState();
        virtual ~PlayingState() = default;
        virtual void Draw(sf::RenderWindow& window) const override;
        virtual void Update(const float deltaTime) override;
        void ResetSessionDelay();
    private:
        /*Contains game objects. First element always is platform, second - ball*/
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        //Deley in seconds before game start from menu or after pause
        float sessionDelay{ 0.f };
    };
}