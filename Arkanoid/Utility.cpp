#include <assert.h>
#include "Utility.h"
#include "Settings.h"
#include "Application.h"
#include "Randomizer.h"

namespace Arkanoid
{
    void TextStyle::Init(const std::string fontName, const sf::Color newColor, const sf::Text::Style newTextStyle, const unsigned int newSize)
    {
        Settings* settings = Application::GetSettings();
#ifdef _DEBUG
        assert(font.loadFromFile(settings->fontPath + fontName));
#else
        font.loadFromFile(settings->fontPath + fontName);
#endif // DEBUG
        color = newColor;
        textStyle = newTextStyle;
        characterSize = newSize;
    }

    void LoadTexture(std::string const& fileName, sf::Texture& texture)
    {
#ifndef NDEBUG
        assert(texture.loadFromFile(Application::GetSettings()->texturePath + fileName));
#else 
        texture.loadFromFile(Application::GetSettings()->texturePath + fileName);
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
}