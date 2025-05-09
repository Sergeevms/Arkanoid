#include <assert.h>
#include "Utility.h"
#include "GameWorld.h"
#include "Application.h"
#include "Randomizer.h"

namespace Arkanoid
{
    void TextStyle::Init(const std::string fontName, const sf::Color newColor, const sf::Text::Style newTextStyle, const unsigned int newSize)
    {
        GameWorld* world = GameWorld::GetWorld();
#ifdef _DEBUG
        assert(font.loadFromFile(world->fontPath + fontName));
#else
        font.loadFromFile(world->fontPath + fontName);
#endif // DEBUG
        color = newColor;
        textStyle = newTextStyle;
        characterSize = newSize;
    }

    void LoadTexture(std::string const& fileName, sf::Texture& texture)
    {
#ifndef NDEBUG
        assert(texture.loadFromFile(GameWorld::GetWorld()->texturePath + fileName));
#else 
        texture.loadFromFile(GameWorld::GetWorld()->texturePath + fileName);
#endif
    }

    RelativePosition RelativePositionByOrientationAndAlignment(const Orientation orientation, const Alignment alignment)
    {
        RelativePosition relativeOrigin = RelativePosition::TopLeft;
        switch (alignment)
        {
        case Alignment::Min:
        {
            relativeOrigin = RelativePosition::TopLeft;
            break;
        }
        case Alignment::Middle:
        {
            relativeOrigin = orientation == Orientation::Vertical ? RelativePosition::TopMiddle : RelativePosition::MiddleLeft;
            break;
        }
        case Alignment::Max:
        {
            relativeOrigin = orientation == Orientation::Vertical ? RelativePosition::TopRight : RelativePosition::BottomLeft;
            break;
        }
        }
        return relativeOrigin;
    }

    sf::Vector2f DirectionVecFromDegree(const float angleDegree)
    {
        const float pi = acos(-1.f);
        const float angleRadian = (pi / 180.f) * angleDegree;
        return { cos(angleRadian), sin(angleRadian) };           
    }

    void ChangeSpriteOpacity(sf::Sprite& sprite, const sf::Uint8 alpha)
    {
        sf::Color color = sprite.getColor();
        color.a = alpha;
        sprite.setColor(color);
    }

    bool CheckRectCollisionWithCircle(const sf::FloatRect platformRect, const sf::Vector2f circlePosition, const float circleRadius)
    {
        auto sqr = [](float x)
            {
                return x * x;
            };

        if (circlePosition.x < platformRect.left)
        {
            return sqr(circlePosition.x - platformRect.left) + sqr(circlePosition.y - platformRect.top) < sqr(circleRadius);
        }

        if (circlePosition.x > platformRect.left + platformRect.width)
        {
            return sqr(circlePosition.x - platformRect.left - platformRect.width) + sqr(circlePosition.y - platformRect.top) < sqr(circleRadius);
        }

        return std::fabs(circlePosition.y - platformRect.top) <= circleRadius;
    }
}