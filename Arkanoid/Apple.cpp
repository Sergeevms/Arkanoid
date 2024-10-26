#include "Apple.h"
#include "Map.h"
#include "Settings.h"

namespace SnakeGame
{
    AppleFactory::AppleFactory()
    {
        LoadTexture("Apple.png", appleTexture);
        LoadTexture("AppleSpecial.png", specialAppleTexture);

        Settings* settings = Settings::GetSettings();

        if (settings->poisionedAppleOn)
        {
            availableSpecialType.push_back(AppleType::Poisned);
        }

        if (settings->disorientAppleOn)
        {
            availableSpecialType.push_back(AppleType::Disorientating);
        }

        if (settings->goldenAppleOn)
        {
            availableSpecialType.push_back(AppleType::Golden);
        }
    }

    std::shared_ptr<Apple> AppleFactory::GenerateNewApple(const Map * map) const
    {
        return GenerateNewApple(map, map->GetRandomEmptyCell());
    }

    std::shared_ptr<Apple> AppleFactory::GenerateNewApple(const Map * map, const sf::Vector2i position) const
    {
        Settings* settings = Settings::GetSettings();
        
        if (availableSpecialType.size() > 0)
        {
            int roll = rand() % 100;
            if (roll < Settings::GetSettings()->specialAppleSpawnChance)
            {
                roll = rand() % availableSpecialType.size();
                switch (availableSpecialType[roll])
                {
                case AppleType::Disorientating:
                {
                    return std::make_shared<DisorientApple>(position, specialAppleTexture);
                    break;
                }
                case AppleType::Poisned:
                {
                    return std::make_shared<PoisendApple>(position, specialAppleTexture);
                    break;
                }
                case AppleType::Golden:
                {
                    return std::make_shared<GoldenApple>(position, specialAppleTexture);
                    break;
                }
                default:
                {
                    return std::make_shared<Apple>(position, appleTexture);
                    break;
                }
                }
            }
        }
        return std::make_shared<Apple>(position, appleTexture);
    }

    DisorientApple::DisorientApple(sf::Vector2i const& mapCell, sf::Texture const& texture) : Apple(mapCell, texture)
    {        
        sprite.setColor(Settings::GetSettings()->disorientAppleColor);
    }

    PoisendApple::PoisendApple(sf::Vector2i const& mapCell, sf::Texture const& texture) : Apple(mapCell, texture)
    {
        sprite.setColor(Settings::GetSettings()->poisionedAppleColor);
    }

    GoldenApple::GoldenApple(sf::Vector2i const& mapCell, sf::Texture const& texture) : Apple(mapCell, texture)
    {
        sprite.setColor(Settings::GetSettings()->goldenAppleColor);
    }
}
