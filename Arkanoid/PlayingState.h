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
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        bool isGameOvered{ false };
        bool sessionStarted{ false };
        float sessionDelay{ 0.f };
        int scoreCount{ 0 };
        sf::Font font;
        sf::Text scoreText;
    };
}