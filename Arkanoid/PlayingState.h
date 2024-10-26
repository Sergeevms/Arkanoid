#pragma once
#include "BaseState.h"
#include "Map.h"
#include "Snake.h"

namespace SnakeGame
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
        //Resets snake moving delay timer
        void resetMovingDelay();
        //Returns true if game can be continued after collisition, false otherwise
        bool CheckColition(sf::Vector2i& cell);
    private:
        bool isGameOvered{ false };
        bool sessionStarted{ false };
        int scoreCount{ 0 };
        float delayBeforeMoving;
        //Time to finish moving after eating apple
        float keepSnakeMovingTime;
        float timeTillGoldenAppleDisapear{ 0 };
        float temporaryWallsTimer{ 0.f };
        bool temporaryWallsPlaced{ false };
        void GenerateApple();
        Map map;
        Snake snake;
        std::shared_ptr<Apple> currentApple;
        std::unique_ptr<AppleFactory> appleFactory;
        sf::Font font;
        sf::Text scoreText;
    };
}