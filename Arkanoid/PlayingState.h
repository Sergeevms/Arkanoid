#pragma once
#include "BaseState.h"
#include <unordered_map>
#include <memory>
#include "IObserver.h"
#include "LevelLoader.h"
#include "BlockFactory.h"
#include "IListDrawable.h"
#include "Utility.h"

namespace Arkanoid
{
    class GameObject;
    class Block;
    enum class BlockType;
    class BlockFactory;
    class LevelLoader;

    class PlayingState :
        public BaseState, public IObserver, public std::enable_shared_from_this<PlayingState>
    {
    public:
        PlayingState();
        virtual ~PlayingState() = default;
        virtual void Draw(sf::RenderWindow& window) const override;
        virtual void Update(const float deltaTime) override;
        virtual void Init() override;
        void ResetSessionDelay();
        void LoadNextLevel();

        virtual void Notify(std::shared_ptr<IObservable> observable) override;
    private:
        void CreateBlocks();
        void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseX, bool& needInverseY);
        /*Contains game objects. First element always is platform, second - ball*/
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::shared_ptr<std::vector<std::shared_ptr<Block>>> blocks;
        //Deley in seconds before game start from menu or after pause
        float sessionDelay = 0.f;
        std::unordered_map<BlockType, std::unique_ptr<BlockFactory>> factories;
        int breakableBlocksCount = 0;
        int nextLevel = 0;
        int currentScore = 0;
        TextStyle scoreTextStyle;
        ListDrawableText scoreText;
        std::unique_ptr<LevelLoader> levelLoader;
    };
}