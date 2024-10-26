#pragma once
#include "BaseState.h"

namespace Arkanoid
{
    class Apple;
    class AppleFactory;
    class PlayingState :
        public BaseState
    {
    public:
        PlayingState();
        virtual ~PlayingState() = default;
        virtual void Draw(sf::RenderWindow& window) const override;
        virtual void Update(const float deltaTime) override;
    private:
        bool isGameOvered{ false };
        bool sessionStarted{ false };
        int scoreCount{ 0 };
        sf::Font font;
        sf::Text scoreText;
    };
}