#pragma once
#include "BaseState.h"

namespace Arkanoid
{
    class Platform;
    class Ball;

    class PlayingState :
        public BaseState
    {
    public:
        PlayingState();
        virtual ~PlayingState() = default;
        virtual void Draw(sf::RenderWindow& window) const override;
        virtual void Update(const float deltaTime) override;
    private:
        std::unique_ptr<Platform> platform;
        std::unique_ptr<Ball> ball;
        bool isGameOvered{ false };
        bool sessionStarted{ false };
        int scoreCount{ 0 };
        sf::Font font;
        sf::Text scoreText;
    };
}