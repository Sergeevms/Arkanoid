#pragma once
#include "BaseState.h"
#include <unordered_map>
#include <memory>
#include "IObserver.h"
#include "LevelLoader.h"
#include "BlockFactory.h"
#include "BonusFactory.h"
#include "IListDrawable.h"
#include "Utility.h"
#include "ISaveable.h"

namespace Arkanoid
{
    class GameObject;
    class Block;
    class BlockSave;
    class BonusSave;
    class GameObjectSave;
    class BallSave;
    enum class BlockType;
    enum class BonusType;
    class LevelLoader;

    class PlayingStateSave : public ISave
    {
    public:
        virtual void SaveToFile(std::ofstream& ostream) const override;
        virtual void LoadFromFile(std::ifstream& ifstream) override;
        PlayingStateSave();
    private:
        friend class PlayingState;
        std::vector<BlockType> blockTypes;
        std::vector<std::shared_ptr<BlockSave>> blockSaves;
        std::vector<BonusType> bonusTypes;
        std::vector<std::shared_ptr<BonusSave>> bonusSaves;
        std::shared_ptr<BallSave> ball;
        std::shared_ptr<GameObjectSave> platform;
        int nextLevel = 0;
        int currentScore = 0;
    };    

    class PlayingState :
        public BaseState, public IObserver, public std::enable_shared_from_this<PlayingState>, public ISaveable
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

        virtual std::shared_ptr<ISave> SaveState() const;
        virtual void SaveState(std::shared_ptr<ISave> save) const override;
        virtual void LoadState(const std::shared_ptr<ISave> save) override;
    private:
        void CreateBlocks(std::shared_ptr<PlayingStateSave> stateSave = nullptr);
        void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseX, bool& needInverseY);
        template <class T>
        void ClearGameObjects();
        /*Contains game objects. First element always is platform, second - ball*/
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        //std::vector<std::shared_ptr<Block>> blocks;
        std::vector<std::shared_ptr<GameObject>> objectsToDestroy;
        //Delay in seconds before game start from menu or after pause
        float sessionDelay = 0.f;
        std::unordered_map<BlockType, std::unique_ptr<BlockFactory>> blockFactories;
        std::unordered_map<BonusType, std::unique_ptr<BonusFactory>> bonusFactories;
        int breakableBlocksCount = 0;
        int nextLevel = 0;
        int currentScore = 0;
        TextStyle scoreTextStyle;
        ListDrawableText scoreText;
        std::unique_ptr<LevelLoader> levelLoader;
    };

    template <class T>
    void PlayingState::ClearGameObjects()
    {
        gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [](auto& object) {
            return std::dynamic_pointer_cast<T>(object);
            }), gameObjects.end());
    };
}