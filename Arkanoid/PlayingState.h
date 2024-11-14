#pragma once
#include "BaseState.h"

namespace Arkanoid
{
    class GameObject;
    class Block;

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
        void CreateBlocks();
        void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseX, bool& needInverseY);
        /*Contains game objects. First element always is platform, second - ball*/
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::vector<std::shared_ptr<Block>> blocks;
        //Deley in seconds before game start from menu or after pause
        float sessionDelay{ 0.f };
    };
}