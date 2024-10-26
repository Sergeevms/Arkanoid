#pragma once
#include "MapObject.h"
#include <vector>

namespace SnakeGame
{
    class Map;

    enum class AppleType
    {
        Default,
        Disorientating,
        Poisned,
        Golden
    };

    class Apple : public MapObject
    {
    public:
        Apple(sf::Vector2i const& mapCell, sf::Texture const& texture) : MapObject(mapCell, texture) {};
    };

    class AppleFactory
    {
    public:
        AppleFactory();
        ~AppleFactory() = default;
        std::shared_ptr<Apple> GenerateNewApple(const Map * map) const;
        std::shared_ptr<Apple> GenerateNewApple(const Map * map, const sf::Vector2i position) const;
    private:
        sf::Texture appleTexture;
        sf::Texture specialAppleTexture;
        std::vector<AppleType> availableSpecialType;
    };

    class DisorientApple : public Apple
    {
    public:
        DisorientApple(sf::Vector2i const& mapCell, sf::Texture const& texture);
        ~DisorientApple() = default;
    };

    class PoisendApple : public Apple
    {
    public:
        PoisendApple(sf::Vector2i const& mapCell, sf::Texture const& texture);
        ~PoisendApple() = default;
    };

    class GoldenApple : public Apple
    {
    public:
        GoldenApple(sf::Vector2i const& mapCell, sf::Texture const& texture);
        ~GoldenApple() = default;
    };
}
